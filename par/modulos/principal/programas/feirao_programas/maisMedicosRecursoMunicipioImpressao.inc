<?php
    include_once '_funcoes_maismedicos.php';

    monta_titulo('Mais Médicos - Recurso Município', recuperaMunicipioEstado());

    echo '<br>';

    $rqmid = verificaExisteQuestionario();

    if ($rqmid == '') {
        $db->sucesso('principal/programas/feirao_programas/maisMedicosAcoesMunicipio', '', 'Não é possivel realizar o Recurso, necessario preencheer as Ações do Município.');
    } else {
        $dadosRecurso = buscaRecudoMaisMedicos($rqmid);
    }

?>

<style type="">
    body {
        width: 595pt;
        height: 842pt;
        overflow: none;
        display: block;
        font-size: 12px;
    }

    @media print {
        .notprint {
            display: none;
        }
    }

</style>

<link type="text/css" rel="stylesheet" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="../includes/listagem.css"/>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">

    function imprimeRecursoMunicipio(){
       window.print();
    }

</script>

<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data">
    <input type="hidden" id="rqmid" name="rqmid" value="<?= $rqmid; ?>"/>
    <input type="hidden" id="rcmid" name="rcmid" value="<?= $dadosRecurso['rcmid'] ?>"/>

    <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td bgcolor="#CCCCCC" colspan="2"> <b>Responsável pelo Recurso do Município</b> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome: </td>
            <td><?= $dadosRecurso['nome_municipio']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> CPF: </td>
            <td><?=$dadosRecurso['usucpfmunicipio']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data de inserção do Recurso:</td>
            <td> <?=$dadosRecurso['rcmdtinclusaomunicipio'];?> </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Recurso do Município</b></td>
        </tr>
        <tr>
            <td colspan="2" style="text-indent: 2%; text-align: justify; line-height:130%;">
                <!--textarea rows="119" id="recurso"-->
                    <?php echo nl2br($dadosRecurso['rcmtextmunicipio']); ?>
                <!--/textarea-->
            </td>
        </tr>
        <tr>
            <td class="subtituloCentro" colspan="2">&nbsp;</td>
        </tr>
    </table>
    <br>
    <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td bgcolor="#CCCCCC" colspan="2"> <b>Responsável pelo Parecer do MEC</b> </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" width="25%"> Nome: </td>
            <td><?=$dadosRecurso['nome_mec']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita"> CPF: </td>
            <td><?=$dadosRecurso['usucpfmec']; ?></td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Data de inserção do Parecer:</td>
            <td> <?=$dadosRecurso['rcmdtinclusaomunicipio']; ?> </td>
        </tr>
        <tr>
            <td bgcolor="#CCCCCC" colspan="2">&nbsp;<b>Descrição do Parecer do MEC</b></td>
        </tr>
        <tr>
            <td colspan="2" style="text-indent: 2%; text-align: justify; line-height:130%;">
                <!--textarea rows="119" id="parecer_mec"-->
                    <?php echo nl2br ($dadosRecurso['rcmparecermec']); ?>
                <!--/textarea-->
            </td>
        </tr>
    </table>
    <br>
    <div class="notprint">
        <table border="0" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td colspan="2" class="SubTituloCentro">
                    <input type="button" value="Imprimir" id="imprimir" name="imprimir" onclick="imprimeRecursoMunicipio();"/>
                </td>
            </tr>
        </table>
    </div>
</form>