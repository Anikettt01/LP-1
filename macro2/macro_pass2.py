call_file = open("CALLS2.txt", "r")
lines = call_file.readlines()
macro_calls = [line.strip() for line in lines]

mnt_file = open("MNT.txt", "r")
lines = mnt_file.readlines()
MNT = [line.strip() for line in lines]

mdt_file = open("MDT.txt", "r")
lines = mdt_file.readlines()
MDT = [line.strip() for line in lines]

kpd_file = open("KPTD.txt", "r")
lines = kpd_file.readlines()
KPDTAB = [line.strip() for line in lines]

print(KPDTAB)


class Process:
    @staticmethod
    def get_macro_def(name):
        for entry in MNT:
            # print("printing Entry")
            # print(entry)
            if entry.split()[0] == name:
                return entry

    @staticmethod
    def macro_mdt(macro_mnt, aptab):
        mdt_index = int(macro_mnt.split()[3]) - 1
        # print("Expanded Code")

        for line in MDT[mdt_index:]:
            if line == "MEND":
                break
            else:
                mdt_line = line.split()
                instruction = mdt_line[0]
                expanded_line = instruction

                for param in mdt_line[1:]:
                    if param.startswith("(P,"):
                        param_index = int(param[3:-1]) - 1
                        expanded_line += f" {aptab[param_index]}"
                    else:
                        expanded_line += f" {param}"

                print(expanded_line)
        print("_")

    @staticmethod
    def prepare_aptab(macro_mnt, macro_call):

        # macro_mnt example = INCR_D 0 3 9 4
        mnt_values = macro_mnt.split()
        num_pp, num_kp, kpdtab_start = int(mnt_values[1]), int(mnt_values[2]), int(mnt_values[4]) - 1

        aptab = []
        # macro call example = INCR_M MEM_VAL=A,INCR_VAL=B,REG=AREG
        call_params = macro_call.split()[1].split(",")
        print(macro_call.split()[0])

        # positional parameters added in aptab
        for param in call_params:
            if "=" not in param:
                aptab.append(param)
        # Adding default values for KP from KPDTAB
        kp_defaults = {}

        # entry example = MEM_VAL no_default
        for entry in KPDTAB[kpdtab_start:kpdtab_start + num_kp]:  # kpdtp to kpdtp + kp
            param_name, param_value = entry.split()
            kp_defaults[param_name] = param_value

        # over riding the default entries from macro call
        for param in call_params:
            if "=" in param:
                param_name, param_value = param.split("=")
                kp_defaults[param_name] = param_value

        # Fill in APTAB with positional and keyword parameters
        for key in kp_defaults:
            aptab.append(kp_defaults[key])

        return aptab


class Main:
    @staticmethod
    def main():
        for macro_call in macro_calls:
            macro_name = macro_call.split()[0]
            macro_entry = Process.get_macro_def(macro_name)
            aptab = Process.prepare_aptab(macro_entry, macro_call)
            Process.macro_mdt(macro_entry, aptab)


Main.main()