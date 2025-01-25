print_usage () {
  echo "Usage: "
  echo "  setup_hw_emu.sh -s on/off                 set emulation mode to on or off"
  echo "" 
}

if [ "$1" = "" ]
then
  print_usage
fi

switch=""
platform="xilinx_vck5000_gen4x8_qdma_2_202220_1"

while true;
do
    case "$1" in
        -s)
            case "$2" in
                on) switch="on"; shift 2;;
                off) switch="off"; break;;
                *) print_usage;;
            esac ;;
        -p)
            platform="$2"; shift 2;;
        "") break;;
        *) print_usage; break ;;
    esac
done

if [ "$switch" = "off" ]
then
  echo "Exit Emulation Mode"
  unset XCL_EMULATION_MODE
else 
  if [ "$switch" = "on" ]
  then
    echo "Generating emulation config file for platform $platform.."
    export XCL_EMULATION_MODE=hw_emu
    emconfigutil --platform $platform
    echo "Enter Hardware Emulation Mode"
  fi
fi
