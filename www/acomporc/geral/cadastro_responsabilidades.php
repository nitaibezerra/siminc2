<?php
/**
 * Consulta de responsábilidades atribuídas ao perfil de um usuário.
 * $Id: cadastro_responsabilidades.php 100161 2015-07-16 13:05:46Z maykelbraz $
 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/acomporc/_constantes.php');
require_once (APPRAIZ . 'includes/library/simec/Listagem.php');
$db = new cls_banco();
$esquema = 'acomporc';

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if (!$pflcod && !$usucpf) {
    ?><font color="red">Requisição inválida</font><?
    exit();
}

$sqlResponsabilidadesPerfil = <<<DML
    SELECT tr.*
    FROM {$esquema}.tprperfil p
    INNER JOIN {$esquema}.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
    WHERE tprsnvisivelperfil = TRUE
        AND p.pflcod = '%s'
    ORDER BY tr.tprdsc
DML;
$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);

$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil) < 1) {
    print "<font color='red'>Não foram encontrados registros</font>";
} else {
    foreach ($responsabilidadesPerfil as $rp) {
        //
        // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
        $sqlRespUsuario = "";
        switch ($rp["tprsigla"]) {
            case 'O': // Unidade Orçamentária
                $cabecalho = array('Código', 'Descrição');
                $aca_prg = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
                    SELECT uni.unicod AS codigo,
                        uni.unicod || ' - ' || uni.unidsc AS descricao
                    FROM {$esquema}.usuarioresponsabilidade ur
                    INNER JOIN public.unidade uni USING(unicod)
                    WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
DML;
                break;
            case 'A': // Ação
                $cabecalho = array('Unidade Orçamentária', 'Ação');
                $aca_prg = 'ações associadas';
                $sqlRespUsuario = <<<DML
                    SELECT DISTINCT
                        aca.unicod ||' - '|| uni.unidsc AS descricao,
                        aca.acacod AS codigo,
                        pf.prftitulo || ' - ' || TO_CHAR(prfinicio, 'DD/MM/YYYY') || ' a ' || TO_CHAR(prffim, 'DD/MM/YYYY') AS periodo
                    FROM acomporc.usuarioresponsabilidade urp
                    INNER JOIN monitora.acao aca on urp.acacod = aca.acacod AND urp.unicod = aca.unicod
                    INNER JOIN public.unidade uni on aca.unicod = uni.unicod
                    JOIN acomporc.periodoreferencia pf ON (pf.prfid = urp.prfid)
                    WHERE urp.usucpf = '%s'
                        AND urp.pflcod = '%s'
                        AND urp.prfid = (SELECT prfid FROM acomporc.periodoreferencia WHERE prsano = '{$_SESSION['exercicio']}' AND prftipo = 'A' ORDER BY prfid DESC LIMIT 1)
                        AND urp.rpustatus = 'A'
                        AND urp.acacod IS NOT NULL
                        AND urp.unicod IS NOT NULL
                    ORDER BY 1,2
DML;
                break;
            case 'S': //Subação
                $cabecalho = array('Subação', 'Período');
                $aca_prg = 'ações associadas';
                $sqlRespUsuario = <<<DML
                    SELECT
                        ur.sbacod || ' - ' || ms.sbatitulo AS subacao,
                        pr.prftitulo || ' - ' || TO_CHAR(pr.prfinicio, 'DD/MM/YYYY') || ' a ' || TO_CHAR(pr.prffim, 'DD/MM/YYYY') AS periodo
                    FROM acomporc.usuarioresponsabilidade ur
                    INNER JOIN monitora.subacao ms USING(sbacod)
                    INNER JOIN acomporc.periodoreferencia pr using (prfid)
                    WHERE ur.usucpf = '%s'
                        AND ur.pflcod = '%s'
                        AND ur.rpustatus = 'A'
                  ORDER BY pr.prfid, ur.sbacod
DML;
                break;
        }

        if (!$sqlRespUsuario)
            continue;
        $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
        $listagem->setQuery($query);
        $listagem->setCabecalho($cabecalho);
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->addCallbackDeCampo('descricao', function($valor){
            return <<<HTML
<p style="text-align:left!important">{$valor}</p>
HTML;
        });
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
}
$db->close();
exit();
