<?php
/**
 * Consulta de responsabilidades atribuídas ao perfil de um usuário.
 * $Id: cadastro_responsabilidades.php 81751 2014-06-18 19:12:21Z maykelbraz $
 */

/**
 *
 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/altorc/_constantes.php');
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if (!$pflcod && !$usucpf) {
    ?><font color="red">Requisição inválida</font><?
    exit();
}

$sqlResponsabilidadesPerfil = <<<DML
SELECT tr.*
  FROM ted.tprperfil p
    INNER JOIN ted.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
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
        // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
        $sqlRespUsuario = "";
        switch ($rp["tprsigla"]) {
            case "A": // AÇÃO
                $aca_prg = "Ações Associadas";
                $sqlRespUsuario = "SELECT DISTINCT
								    a.prgcod || '.' || a.acacod  AS codigo, a.acadsc AS descricao, a.prgid, a.acaid, a.acacod, u.rpustatus AS status
								   FROM
								    ted.usuarioresponsabilidade u
								    INNER JOIN elabrev.ppaacao_proposta a ON a.acaid = u.acaid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND

								    u.rpustatus='A'
								   ORDER BY
								    a.prgcod,
								    a.acacod";
                break;
            case "P": // PROGRAMAS
                $aca_prg = "Programas Associados";
                $sqlRespUsuario = "SELECT DISTINCT
									p.prgcod AS codigo, p.prgdsc AS descricao, u.rpustatus AS status
								   FROM
								    ted.usuarioresponsabilidade u
								    INNER JOIN elabrev.ppaprograma_proposta p ON p.prgid = u.prgid
								   WHERE
								    u.usucpf = '%s' AND
								    u.pflcod = '%s' AND

								    u.rpustatus='A'";
                break;
            case "U": // Unidades
                $aca_prg = "Unidades Associadas";
                $sqlRespUsuario = "SELECT DISTINCT
								   u.unicod AS codigo, u.unidsc AS descricao, ur.rpustatus AS status
								   FROM
								    ted.usuarioresponsabilidade ur
									INNER JOIN unidade u ON u.unicod = ur.unicod
								   WHERE
								    ur.usucpf = '%s' AND
								    ur.pflcod = '%s' AND

								    ur.rpustatus='A'";

                break;
            case "G": // Unidades
                $aca_prg = "Unidades Associadas";
                $sqlRespUsuario = "	SELECT DISTINCT
								   		u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
								   	FROM
								    	ted.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
								   	WHERE
									    ur.usucpf = '%s' AND
									    ur.pflcod = '%s' AND

									    ur.rpustatus='A'";
                break;
            case "O": // Unidades
                $aca_prg = "Coordenações Vinculadas";
                $sqlRespUsuario = "SELECT DISTINCT
										u.cooid AS codigo, u.coodsc AS descricao, ur.rpustatus AS status
									FROM
										ted.usuarioresponsabilidade ur
									INNER JOIN ted.coordenacao u ON u.cooid = ur.cooid
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND

										ur.rpustatus='A'";
                break;
            case "S": // Unidades
                $aca_prg = "Secretarias Vinculadas";
                $sqlRespUsuario = "SELECT DISTINCT
										u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
									FROM
										ted.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND

										ur.rpustatus='A'";
                break;
            case "D": // Unidades
                $aca_prg = "Diretorias Vinculadas";
                $sqlRespUsuario = "SELECT DISTINCT
										u.dircod AS codigo, u.dirdsc AS descricao, ur.rpustatus AS status
									FROM
										ted.usuarioresponsabilidade ur
									INNER JOIN ted.diretoria u ON u.dircod = ur.dircod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND

										ur.rpustatus='A'";
                break;
            case "C": // Unidades
                $aca_prg = "Secretarias Vinculadas";
                $sqlRespUsuario = "SELECT DISTINCT
										u.ungcod AS codigo, u.ungdsc AS descricao, ur.rpustatus AS status
									FROM
										ted.usuarioresponsabilidade ur
									INNER JOIN public.unidadegestora u ON u.ungcod = ur.ungcod
									WHERE
										ur.usucpf = '%s' AND
										ur.pflcod = '%s' AND

										ur.rpustatus='A'";
                break;
        }
        if (!$sqlRespUsuario) {
            continue;
        }
        $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
        $respUsuario = $db->carregar($query);
        if (!$respUsuario || @count($respUsuario) < 1) {
            print "<center><font color='red'>Não existem {$respdsc} a este Perfil.</font></center>";
        } else {
            ?>
            <table width="100%" border="0" cellspacing="0" cellpadding="0" align="center" style="width:100%; border: 0px; color:#006600;">
                <tr>
                    <td colspan="3"><?= $rp["tprdsc"] ?></td>
                </tr>
                <tr style="color:#000000;">
                    <td valign="top" width="12">&nbsp;</td>
                    <td valign="top">Código</td>
                    <td valign="top">Descrição</td>
                </tr>
            <? foreach ($respUsuario as $ru): ?>
                <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
                    <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
                    <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?php echo $ru["codigo"]; ?></td>
                    <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?= $ru["descricao"] ?></td>
                </tr>
            <? endforeach; ?>
                <tr>
                    <td colspan="4" align="right" style="color:000000;border-top: 2px solid #000000;">Total: (<?= @count($respUsuario) ?>)</td>
                </tr>
            </table>
            <?
        }
    }
}
$db->close();
exit();
