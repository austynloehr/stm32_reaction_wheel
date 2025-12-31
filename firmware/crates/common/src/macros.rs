/// Macro to define a strongly typed wrapper around a 3D vector.
#[macro_export]
macro_rules! define_3d_mux {
    ($name:ident, $doc:literal) => {
        #[doc = $doc]
        #[derive(Clone, Copy, PartialEq, Debug)]
        pub struct $name(nalgebra::Vector3<f32>);

        impl defmt::Format for $name {
            fn format(&self, f: defmt::Formatter) {
                defmt::write!(
                    f,
                    "{}({=f32}, {=f32}, {=f32})",
                    stringify!($name),
                    self.0.x,
                    self.0.y,
                    self.0.z
                )
            }
        }

        impl $name {
            pub const fn new(x: f32, y: f32, z: f32) -> Self {
                Self(nalgebra::Vector3::new(x, y, z))
            }

            pub const fn from_vec(vec: nalgebra::Vector3<f32>) -> Self {
                Self(vec)
            }

            /// Safely allow access to vector data
            pub fn data(&self) -> &nalgebra::Vector3<f32> {
                &self.0
            }
        }
    };
}
