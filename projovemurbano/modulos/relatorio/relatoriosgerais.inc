<?php

checkAno();

    if (isset($_REQUEST['buscar'])) {
        include $_REQUEST['relatorio'];
        exit;
    }

    include APPRAIZ . 'includes/Agrupador.php';
    include APPRAIZ . 'includes/cabecalho.inc';
    print '<br/>';
    include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
    echo "<br>";

    monta_titulo("Relatórios Personalizados - PROJOVEM URBANO", "Para visualizar o relatório, basta selecionar e clicar no botão VISUALIZAR");
    
?>
<script type="text/javascript">
    function exibirRelatorio() {
        if (document.getElementById('relatorio').value != '') {
            var formulario = document.formulario;
            // submete formulario
            formulario.target = 'relatoriopersonlizadosprojovem';
            var janela = window.open('', 'relatoriopersonlizadosprojovem', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1');
            formulario.submit();
            janela.focus();
        } else {
            alert("Selecione um relatório");
            return false;
        }
    }
</script>

<form action="" method="post" name="formulario">
    <input type="hidden" name="buscar" value="1"/>
    <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
        <tr>
            <td class="SubTituloDireita" valign="top">Relatórios :</td>
            <td>
                <?
                $relatorios = array(/*0 => array('codigo' => 'relatgeral_adesao.inc', 'descricao' => 'Relatório de Adesão'),*/
                    0 => array('codigo' => 'relatgeral_secretarioscad.inc', 'descricao' => 'Relatório de Secretários Cadastrados')
                );
                $db->monta_combo('relatorio', $relatorios, 'S', 'Selecione', '', '', '', '400', 'S', 'relatorio');
                ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" valign="top">&nbsp;</td>
            <td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();"/></td>
        </tr>
    </table>
