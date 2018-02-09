<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "9024M");
ini_set("default_socket_timeout", "70000000");

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

$_REQUEST['baselogin'] = "simec_espelho_producao";//simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';

$db = new cls_banco();

include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/modelo/obras2/Obras.class.inc";
include_once APPRAIZ . "includes/classes/dateTime.inc";


$sql = "
    UPDATE obras2.obras SET stiid = 2 WHERE obrid IN (
        SELECT
          o.obrid
        FROM obras2.obras o
        LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
        LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
        LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
        JOIN (SELECT
            \"ID Obra\" as obrid,
            \"Número do Termo\" as termo,
            CASE WHEN \"Fonte\" = 'PAR' THEN
            date_part('days',(SELECT (date_trunc('MONTH', ((string_to_array(\"Fim Vigência Termo\", '/'))[2] ||'-'|| (string_to_array(\"Fim Vigência Termo\", '/'))[1] ||'-'|| '01')::date) + INTERVAL '1 MONTH - 1 day')::date)) || '/' || \"Fim Vigência Termo\"
            ELSE
            \"Fim Vigência da Obra\"
            END fim_vigencia
        FROM obras2.vm_termo_obras
        ) as t ON t.obrid = o.obrid
        WHERE
        o.obridpai IS NULL
        AND o.obrstatus = 'A'
        AND (((string_to_array(t.fim_vigencia, '/'))[3] || '-' || (string_to_array(t.fim_vigencia, '/'))[2] || '-' || (string_to_array(t.fim_vigencia, '/'))[1])::date ) < NOW()

        UNION

        SELECT
            o.obrid
        FROM obras2.obras o
        INNER JOIN painel.dadosconvenios d ON d.dcoprocesso = Replace(Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),';',''),'/',''),'-','')
        WHERE o.obrstatus = 'A' AND o.obridpai IS NULL AND o.tooid = 2 AND dcodatafim < NOW()

	)
	";

$db->executar($sql);
$db->commit();