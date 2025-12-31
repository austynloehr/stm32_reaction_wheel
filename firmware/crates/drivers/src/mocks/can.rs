use common::types::AsyncCanHal;
use embedded_can::{Frame, Id};
use mockall::mock;

#[derive(Debug, Clone, PartialEq)]
pub struct TestError;
impl core::fmt::Display for TestError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "TestError")
    }
}

#[derive(Debug, Clone, PartialEq)]
pub struct TestFrame {
    pub id: Id,
    pub data: [u8; 8],
    pub len: usize,
}

impl Frame for TestFrame {
    fn new(id: impl Into<Id>, data: &[u8]) -> Option<Self> {
        let mut bytes = [0u8; 8];
        if data.len() > 8 {
            return None;
        }
        bytes[..data.len()].copy_from_slice(data);
        Some(Self {
            id: id.into(),
            data: bytes,
            len: data.len(),
        })
    }
    fn new_remote(_id: impl Into<Id>, _len: usize) -> Option<Self> {
        None // Not used
    }
    fn is_extended(&self) -> bool {
        match self.id {
            Id::Extended(_) => true,
            _ => false,
        }
    }
    fn is_remote_frame(&self) -> bool {
        false
    }
    fn id(&self) -> Id {
        self.id
    }
    fn dlc(&self) -> usize {
        self.len
    }
    fn data(&self) -> &[u8] {
        &self.data[..self.len]
    }
}

mock! {
    pub Can {}

    impl AsyncCanHal for Can {
        type Error = TestError;
        type Frame = TestFrame;

        async fn write(&mut self, frame: &TestFrame) -> Result<(), TestError>;
        async fn read(&mut self) -> Result<TestFrame, TestError>;
    }
}
