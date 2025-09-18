import type { Route } from "./+types/device";
import Device from "../deivce";

export function meta({}: Route.MetaArgs) {
  return [
    { title: "Esp32 Led Control Sample" },
    { name: "description", content: "Welcome to React Router!" },
  ];
}

export default function Home() {
  
  return <Device />;
}