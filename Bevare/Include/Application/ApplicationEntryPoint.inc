extern eastl::shared_ptr<bevare::UApplication> CreateApplication(
    const bevare::FCommandLineArgs& args);

int main(int argc, char** argv) {
  auto app = CreateApplication({argv, argc});

  app->Run();

  return 0;
};
