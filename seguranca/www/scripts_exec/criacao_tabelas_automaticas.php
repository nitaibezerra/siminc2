<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
// require_once "../../global/config.inc";

require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";

//eduardo - envio SMS pendecias de obras - PAR
//http://simec-local/seguranca/scripts_exec/par_enviaSMS_pendenciasAtualizacaoObras.php
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 4;

$db = new cls_banco();

$complemento = date('Y_m');
$complemento = date('Y_m', strtotime("+80 month"));


/****************************************************
*                       AUDITORIA                   *
/***************************************************/
$sql = "SELECT 1
        FROM pg_catalog.pg_tables
        WHERE schemaname = 'auditoria'
        and tablename = 'auditoria_{$complemento}'";
$existeTabela = $db->pegaUm($sql);

if (!$existeTabela) {
    $sql = "CREATE TABLE auditoria.auditoria_{$complemento}
            (
              audid serial NOT NULL,
              usucpf character(11),
              mnuid integer,
              audsql text,
              audtabela character varying(100),
              audtipo character(1),
              audip character varying(20),
              auddata timestamp without time zone DEFAULT now(),
              audmsg text,
              sisid integer,
              audscript character varying(5000),
              CONSTRAINT pk_auditoria_{$complemento} PRIMARY KEY (audid)
            )";

        $db->executar($sql);
        $db->commit();
}

/****************************************************
*                     ESTATÍSTICA                   *
/***************************************************/
$sql = "SELECT 1
        FROM pg_catalog.pg_tables
        WHERE schemaname = 'estatistica'
        and tablename = 'estatistica_{$complemento}'";
$existeTabela = $db->pegaUm($sql);

if (!$existeTabela) {

    $sql = "CREATE TABLE estatistica.estatistica_{$complemento}
            (
              mnuid integer NOT NULL,
              usucpf character(11) NOT NULL,
              estdata timestamp without time zone NOT NULL DEFAULT now(),
              esttempoexec double precision NOT NULL,
              estsession character varying(32),
              sisid integer,
              estmemusa numeric,
              estip character(20)
            );

            CREATE INDEX idx_estatistica_mnuid_{$complemento}
              ON estatistica.estatistica_{$complemento}
              USING btree
              (mnuid)
            TABLESPACE tblspc_dbsimec_idx;

            CREATE INDEX idx_estdata_{$complemento}
              ON estatistica.estatistica_{$complemento}
              USING btree
              (estdata)
            TABLESPACE tblspc_dbsimec_idx;

            CREATE INDEX idx_sisid_estatistica_{$complemento}
              ON estatistica.estatistica_{$complemento}
              USING btree
              (sisid)
            TABLESPACE tblspc_dbsimec_idx;

            CREATE INDEX idx_usucpf_estatistica_{$complemento}
              ON estatistica.estatistica_{$complemento}
              USING btree
              (usucpf)
            TABLESPACE tblspc_dbsimec_idx;

            DROP VIEW seguranca.usuariosonline;

            CREATE OR REPLACE VIEW seguranca.usuariosonline AS
             SELECT us.usucpf, u.usunome, u.usuemail, u.usufoneddd, u.usufonenum,
                    CASE
                        WHEN u2.unidsc IS NULL THEN u.orgao
                        ELSE u2.unidsc
                    END AS unidsc, mun.mundescricao, mun.estuf, us.sisid, z.mnuid, m.mnudsc, z.estdtultacesso AS ultimoacesso, us.susdataultacesso AS datalogin, to_char(now() - us.susdataultacesso::timestamp with time zone, 'HH24:MI:SS'::text) AS tempologado
               FROM seguranca.usuario_sistema us
               JOIN ( SELECT e.usucpf, e.sisid, max(e.estdata) AS estdtultacesso, max(e.mnuid) AS mnuid
                       FROM estatistica.estatistica_{$complemento} e
                      WHERE e.estdata >= (now() - '00:15:00'::interval)
                      GROUP BY e.usucpf, e.sisid) z ON z.usucpf = us.usucpf AND z.sisid = us.sisid
               JOIN seguranca.menu m ON z.mnuid = m.mnuid
               JOIN seguranca.usuario u ON z.usucpf = u.usucpf
               LEFT JOIN territorios.municipio mun ON u.muncod::bpchar = mun.muncod
               LEFT JOIN unidade u2 ON u.unicod = u2.unicod AND u2.unitpocod = 'U'::bpchar;

            ";

        $db->executar($sql);
        $db->commit();
}

echo 'FIM';

