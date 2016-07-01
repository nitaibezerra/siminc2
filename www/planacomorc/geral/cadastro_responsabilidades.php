<?
/*
  Sistema Simec
  Setor responsável: SPO-MEC
  Desenvolvedor: Equipe Consultores Simec
  Analista: Gilberto Arruda Cerqueira Xavier
  Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
  Módulo:cadastro_usuario_elaboracao_responsabilidades.php

 */
include "config.inc";
header('Content-Type: text/html; charset=iso-8859-1');
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
$db = new cls_banco();

$usucpf = $_REQUEST["usucpf"];
$pflcod = $_REQUEST["pflcod"];

if (!$pflcod && !$usucpf) {
    ?><font color="red">Requisição inválida</font><?
    exit();
}

$sqlResponsabilidadesPerfil = "SELECT tr.*
                                 FROM planacomorc.tprperfil p
                                   INNER JOIN planacomorc.tiporesponsabilidade tr ON p.tprcod = tr.tprcod
                                 WHERE tprsnvisivelperfil = TRUE
                                   AND p.pflcod = '%s'
	                         ORDER BY tr.tprdsc";



$query = sprintf($sqlResponsabilidadesPerfil, $pflcod);

$responsabilidadesPerfil = $db->carregar($query);
if (!$responsabilidadesPerfil || @count($responsabilidadesPerfil) < 1) {
    print "<font color='red'>Não foram encontrados registros</font>";
} else {
    foreach ($responsabilidadesPerfil as $rp) {
        // monta o select com codigo, descricao e status de acordo com o tipo de responsabilidade (ação, programas, etc)
        $sqlRespUsuario = "";
        switch ($rp["tprsigla"]) {
            case 'A': // Ações
                $aca_prg = "ações associadas";
                $sqlRespUsuario = "SELECT aca.codigo,
                                                  case when pr.inicio_validade is not null then
                                                    dae.descricao || ' <p><b>Período: ' || pr.titulo || ' - ' || TO_CHAR(pr.inicio_validade,'DD/MM/YYYY') || ' a ' || TO_CHAR(pr.fim_validade,'DD/MM/YYYY') || '</b></p>'
                                                  else dae.descricao end AS descricao ,
                                                  ur.rpustatus AS status
                                             FROM planacomorc.usuarioresponsabilidade ur
                                               INNER JOIN planacomorc.acao_programatica apr using(id_acao_programatica)
                                               inner join planacomorc.acao aca using(id_acao)
                                               inner join planacomorc.dados_acao_exercicio dae using(id_acao)
                                               left join planacomorc.periodo_referencia pr on pr.id_periodo_referencia = ur.id_periodo_referencia
                                             WHERE ur.usucpf = '%s'
                                               AND ur.pflcod = '%s'
                                               AND ur.rpustatus='A'
                                               AND apr.id_exercicio = {$_SESSION['exercicio']}
                                               AND dae.id_exercicio = {$_SESSION['exercicio']} ";

                break;
            case 'S': // Subações
                $aca_prg = "subações associadas";
                $sqlRespUsuario = "SELECT sac.codigo,
                                                  sac.sigla AS descricao,
                                                  ref.titulo,
                                                  ur.rpustatus AS status
                                             FROM planacomorc.usuarioresponsabilidade ur
                                               INNER JOIN planacomorc.subacao sac using(id_subacao)
                                               INNER JOIN planacomorc.periodo_referencia ref USING(id_periodo_referencia)
                                             WHERE ur.usucpf = '%s'
                                               AND ur.pflcod = '%s'
                                               AND ur.rpustatus = 'A'
                                               AND sac.id_exercicio = ref.id_exercicio

";
                break;
            case 'G': // Unidade gestora
                $aca_prg = 'unidades gestoras associadas';
                $sqlRespUsuario = <<<DML
SELECT ung.ungcod AS codigo,
       ung.ungcod || ' - ' || ung.ungdsc AS descricao,
       ur.rpustatus AS status
  FROM planacomorc.usuarioresponsabilidade ur
    INNER JOIN public.unidadegestora ung USING(ungcod)
  WHERE ur.usucpf = '%s'
    AND ur.pflcod = '%s'
    AND ur.rpustatus = 'A'
DML;
                break;
            case 'O': // Unidade gestora
                $aca_prg = 'unidades orçamentárias associadas';
                $sqlRespUsuario = <<<DML
SELECT uni.unicod AS codigo,
       uni.unicod || ' - ' || uni.unidsc AS descricao,
       ur.rpustatus AS status
  FROM planacomorc.usuarioresponsabilidade ur
    INNER JOIN public.unidade uni USING(unicod)
  WHERE ur.usucpf = '%s'
    AND ur.pflcod = '%s'
    AND ur.rpustatus = 'A'
DML;
                break;
            case 'T':
                $aca_prg = 'ações do TCU relacionadas';
                $sqlRespUsuario = <<<DML
SELECT rlg.rlgid AS codigo,
       rlg.unicod || '.' || rlg.acacod AS descricao,
       rpu.rpustatus AS status
  FROM planacomorc.usuarioresponsabilidade rpu
    INNER JOIN planacomorc.relgestao rlg USING(rlgid)
  WHERE rpu.usucpf = '%s'
    AND rpu.pflcod = '%s'
    AND rpu.rpustatus = 'A'
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
                    <?php if ('S' == $rp["tprsigla"]): ?>
                    <td valign="top">Período de Referência</td>
                    <?php endif?>
                </tr>
            <? foreach ($respUsuario as $ru): ?>
                <tr onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='F7F7F7';" bgcolor="F7F7F7">
                    <td valign="top" width="12" style="padding:2px;"><img src="../imagens/seta_filho.gif" width="12" height="13" alt="" border="0"></td>
                    <td valign="top" width="90" style="border-top: 1px solid #cccccc; padding:2px; color:#003366;" nowrap><?php echo $ru["codigo"]; ?></td>
                    <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?= $ru["descricao"] ?></td>
                    <?php if ('S' == $rp["tprsigla"]): ?>
                    <td valign="top" width="290" style="border-top: 1px solid #cccccc; padding:2px; color:#006600;"><?= $ru["titulo"] ?></td>
                    <?php endif?>
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
?>