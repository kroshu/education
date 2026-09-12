from launch_ros.actions import Node

from launch import LaunchDescription
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration


def generate_launch_description():
    declare_to_greet = DeclareLaunchArgument(
        "to_greet",
        default_value="world",
        description="Name of the person to greet",
    )

    declare_greeting = DeclareLaunchArgument(
        "greeting",
        default_value="Hello",
        description="Greeting message",
    )

    publisher = Node(
        package="topic_example",
        executable="talker",
        name="talker",
        parameters=[
            {
                "to_greet": LaunchConfiguration("to_greet"),
                "greeting": LaunchConfiguration("greeting"),
            }
        ],
    )

    subscriber = Node(
        package="topic_example",
        executable="listener",
        name="listener",
    )

    return LaunchDescription(
        [
            declare_to_greet,
            declare_greeting,
            publisher,
            subscriber,
        ]
    )
