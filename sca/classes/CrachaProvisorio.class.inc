<?php

class CrachaProvisorio extends Modelo{

    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = "sca.crachaprovisorio";

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array("cpsid");

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos     = array('cpsid'              => null,
                                       'nu_matricula_siape' => null,
                                       'cpsdatentrada'      => null,
                                       'cpsdatsaida'        => null,
                                       'vstnumcracha'       => null,
                                       'usucpfentrada'      => null,
                                       'usucpfsaida'        => null
                                      );

    protected $sqServer = null;
    protected $sqUser   = null;
    protected $sqPass   = null;
    protected $sqDB     = null;
    protected $sqLink   = null;

    public function __construct(){

        parent::__construct();

        if(IS_PRODUCAO){
            $sqServer = "mecsrv14";
            $sqUser   = "sysdbsca";
            $sqPass   = "sysdbsca";
            $sqDB     = "dbscf";
        } else{
            $sqServer = "mecsrv22";
            $sqUser   = "sysdbsca";
            $sqPass   = "sysdbsca";
            $sqDB     = "dbscf";
        }

        $sqLink = mssql_connect($sqServer, $sqUser, $sqPass)
            or die("Couldn't connect to SQL Server on $sqServer");

        mssql_select_db($sqDB, $sqLink)
            or die("Couldn't open database $sqDB");
    }

    public function __destruct(){

        if($sqLink){
            mssql_close($link);
        }
    }

    public function cadastrarCartaoProvisorio($nu_cpf, $nu_matricula_siape, $vstnumcracha){

        $nu_cpf = formatar_cpf($nu_cpf);
        $vstnumcracha = "1500002" . str_pad($vstnumcracha, 5, "0", STR_PAD_LEFT);

        $sql = "select count(*)
                  from dbscf..tb_scf_cadastro_cartao
                 where nu_cpf = '$nu_cpf'";

        $row = mssql_fetch_row(mssql_query($sql));

        if($row[0] == 0){
            $sql = "insert into dbscf..tb_scf_cadastro_cartao(
                        nu_cpf, nu_matricula_siape, co_grupo_apuracao)
                    values ('$nu_cpf', $nu_matricula_siape, '01')";

            mssql_query($sql);
        }

        $sql = "insert into dbscf..tb_scf_associacao_cartao(
                    nu_cpf, nu_cartao, nu_matricula_siape, dt_ingresso_cartao)
                values ('$nu_cpf', '$vstnumcracha', $nu_matricula_siape, cast(cast(getdate() as char(11)) + ' 00:00:01' as datetime))";

        mssql_query($sql);
    }

    public function baixarCartaoProvisorio($nu_matricula_siape, $vstnumcracha){

        $vstnumcracha = "1500002" . str_pad($vstnumcracha, 5, "0", STR_PAD_LEFT);

        $sql = "update dbscf..tb_scf_associacao_cartao
                   set dt_saida_cartao = cast(cast(getdate() as char(11)) + ' 23:59:59' as datetime)
                 where nu_cartao = '$vstnumcracha'
                   and nu_matricula_siape = $nu_matricula_siape
                   and dt_saida_cartao is null";

        mssql_query($sql);
    }

    public function verificarExpedienteTrabalho(){
        $sql = "select datepart(weekday, getdate()) as dia_semana
                      ,count(co_dia) as feriado
                  from dbscf..tb_scf_feriados
                 where convert(varchar(10), co_dia, 102) = convert(varchar(10), getdate(), 102)
                   and tp_feriado = 'F'";

        $row = mssql_fetch_row(mssql_query($sql));

        $diaSemana = $row[0];
        $feriado = $row[1];

        if($diaSemana == 1 || $diaSemana == 7){
            return SCA_EXPEDIENTE_FINAL_SEMANA;
        } elseif($feriado > 0){
            return SCA_EXPEDIENTE_FERIADO;
        } else{
            $horaAtual = time();
            $horaAnterior = mktime(7, 0, 0, date("m", $horaAtual), date("d", $horaAtual), date("Y", $horaAtual));
            $horaPosterior = mktime(20, 0, 0, date("m", $horaAtual), date("d", $horaAtual), date("Y", $horaAtual));

            if($horaAtual >= $horaAnterior && $horaAtual <= $horaPosterior){
                return SCA_EXPEDIENTE_NORMAL;
            } else{
                return SCA_EXPEDIENTE_FORA_HORARIO;
            }
        }
    }
}