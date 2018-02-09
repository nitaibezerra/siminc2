<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$obras = array();

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/RegistroAtividade.class.inc";
include_once APPRAIZ . "includes/classes/Sms.class.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 98;

/****************************************
*				PREFEITO				*
****************************************/

$conteudo = "MAIS EDUCAÇÃO: Planos de atendimento de escolas vinculadas a essa secretaria aguardam avaliação, solicitamos agilizar análise.";

$dados = getDadosEstadual();
enviarNotificacoes($dados, $conteudo);

$dados = getDadosMunicipal();
enviarNotificacoes($dados, $conteudo);


//registraAtividade($obras);

echo "FIM";

function getDadosEstadual()
{
	$db = new cls_banco();

    $sql = "select celular
            From dblink('host= user= password= port= dbname=',
                '
SELECT DISTINCT u.usufoneddd || u.usufonenum AS celular
FROM
    seguranca.usuario u
    INNER JOIN seguranca.perfilusuario pu
                ON pu.usucpf = u.usucpf AND pu.pflcod in (820)
    INNER JOIN seguranca.perfil pe
                ON pe.pflcod = pu.pflcod
    LEFT JOIN pddeinterativo.pessoa pes
                ON pes.usucpf = u.usucpf and pes.pflcod = 820
    INNER JOIN pddeinterativo.usuarioresponsabilidade ur
                ON ur.pflcod = 820
                AND ur.usucpf = u.usucpf
                AND ur.rpustatus=''A''
    INNER JOIN seguranca.usuario_sistema ususis
                ON u.usucpf = ususis.usucpf
                AND ususis.susstatus = ''A''
                AND ususis.sisid = 143
    LEFT JOIN territorios.municipio m
                ON m.muncod = ur.muncod
    LEFT JOIN territorios.estado e
                ON e.estuf = m.estuf
    LEFT JOIN territorios.estado ee
                ON ee.estuf = ur.estuf
    LEFT JOIN public.tipoorgao tpo
                ON tpo.tpocod = u.tpocod
WHERE
   ususis.suscod=''A''
AND ur.muncod IS NULL
AND ur.estuf in (
    select  distinct lp.estuf as uf
    from pdeescola.memaiseducacao as me
    inner join pddeinterativo.listapdeinterativo lp on lp.pdicodinep = me.entcodent
    left join workflow.documento as doc on doc.docid = me.docid
    left join workflow.estadodocumento as est on est.esdid = doc.esdid
    where memanoreferencia = 2014
    and est.esddsc = ''Em avaliação na secretaria municipal ou estadual''
    and lp.pdiesfera = ''Estadual''
)


                 '
            )
            AS pd (
            celular text
            )
             ";

 	return $db->carregar($sql);
}

function getDadosMunicipal()
{
	$db = new cls_banco();

    $sql = "select celular
            From dblink('host= user= password= port= dbname=',
                '
SELECT DISTINCT u.usufoneddd || u.usufonenum AS celular
FROM
    seguranca.usuario u
    INNER JOIN seguranca.perfilusuario pu
                ON pu.usucpf = u.usucpf
                AND pu.pflcod in (821)
    INNER JOIN seguranca.perfil pe
                ON pe.pflcod = pu.pflcod
    LEFT JOIN pddeinterativo.pessoa pes
                ON pes.usucpf = u.usucpf
                AND pes.pflcod = 821
    INNER JOIN pddeinterativo.usuarioresponsabilidade ur
                ON ur.pflcod = 821
                AND ur.usucpf = u.usucpf
                AND ur.rpustatus=''A''
    INNER JOIN seguranca.usuario_sistema ususis
                ON u.usucpf = ususis.usucpf
                AND ususis.susstatus = ''A''
                AND ususis.sisid = 143
    LEFT JOIN territorios.municipio m
                ON m.muncod = ur.muncod
    LEFT JOIN territorios.estado e
                ON e.estuf = m.estuf
    LEFT JOIN territorios.estado ee
                ON ee.estuf = ur.estuf
    LEFT JOIN public.tipoorgao tpo
               ON tpo.tpocod = u.tpocod
WHERE
    ususis.suscod = ''A''
    AND ur.estuf IS NULL
AND m.muncod in

(
select  distinct lp.muncod
from pdeescola.memaiseducacao as me
inner join pddeinterativo.listapdeinterativo lp on lp.pdicodinep = me.entcodent
left join workflow.documento as doc on doc.docid = me.docid
left join workflow.estadodocumento as est on est.esdid = doc.esdid
where memanoreferencia = 2014
and est.esddsc = ''Em avaliação na secretaria municipal ou estadual''
and lp.pdiesfera = ''Municipal''

)
                 '
            )
            AS pd (
            celular text
            )

             ";

 	return $db->carregar($sql);
}

function enviarNotificacoes($dados, $conteudo)
{
    $db = new cls_banco();

    $aCelularEnvio = array();
    $aCelularEnvio = array(
        '556191434894', // Daniel
        '556181054537', // Orion
        '556199440097', // Leandro
        '556186183665', // Luiz
    );
    foreach ($dados as $contato) {
        $aCelularEnvio[] = 55 . str_replace(array(' ', '-'), array('', ''), $contato['celular']);
    }

    $aCelularEnvio = array_unique($aCelularEnvio);

    $data = date('Y-m-d') . ' 08:00:00';

    $sms = new Sms();
    $sms->enviarSms($aCelularEnvio, $conteudo, $data);
}

?>
