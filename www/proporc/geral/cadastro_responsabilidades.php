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
  FROM proporc.tprperfil p
    INNER JOIN proporc.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
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
            case 'C': // -- Coluna (matriz)
                $respdsc = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
SELECT rpu.mtrid AS codigo,
       gpm.gpmdsc || ': ' || mtr.mtrdsc AS descricao,
       rpu.rpustatus AS status
  FROM proporc.usuarioresponsabilidade rpu
    INNER JOIN elabrev.matriz mtr USING(mtrid)
    INNER JOIN elabrev.grupomatriz gpm USING(gpmid)
  WHERE rpu.usucpf = '%s'
    AND rpu.pflcod = '%s'
    AND rpu.rpustatus = 'A'
DML;
                break;
            case 'O': // Unidade gestora
                $aca_prg = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao,
       ur.rpustatus AS status
  FROM proporc.usuarioresponsabilidade ur
    INNER JOIN public.unidade uni USING(unicod)
  WHERE ur.usucpf = '%s'
    AND ur.pflcod = '%s'
    AND ur.rpustatus = 'A'
DML;
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
