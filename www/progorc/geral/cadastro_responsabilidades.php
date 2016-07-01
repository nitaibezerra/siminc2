<?php
/**
 * Consulta de responsábilidades atribuídas ao perfil de um usuário.
 * $Id: cadastro_responsabilidades.php 76846 2014-03-11 12:43:13Z werteralmeida $
 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require (APPRAIZ . 'www/progorc/_constantes.php');
$db = new cls_banco();
$esquema = 'progorc';

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if (!$pflcod && !$usucpf) {
    ?><font color="red">Requisição inválida</font><?
    exit();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
                                 FROM {$esquema}.tprperfil p
                                   INNER JOIN {$esquema}.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
                                 WHERE tprsnvisivelperfil = TRUE
                                   AND p.pflcod = '%s'
	                         ORDER BY tr.tprdsc";
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
            case 'O': // Unidade gestora
                $aca_prg = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao,
       ur.rpustatus AS status
  FROM {$esquema}.usuarioresponsabilidade ur
    INNER JOIN public.unidade uni USING(unicod)
  WHERE ur.usucpf = '%s'
    AND ur.pflcod = '%s'
    AND ur.rpustatus = 'A'
DML;
        }
        if (!$sqlRespUsuario)
            continue;
        $query = vsprintf($sqlRespUsuario, array($usucpf, $pflcod));
        $respUsuario = $db->carregar($query);
        if (!$respUsuario || @count($respUsuario) < 1) {
            print "<center><font color='red'>Não existem $aca_prg a este Perfil.</font></center>";
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
