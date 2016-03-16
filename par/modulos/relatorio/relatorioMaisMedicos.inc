<?PHP

    include_once '_funcoes_maismedicos.php';

    if ($_REQUEST['requisicao'] == "relatorio") {
        gerarRelatorio($_POST);
        exit();
    }

    include APPRAIZ . 'includes/cabecalho.inc';
    $titulo_modulo = "Relatório - Mais Médicos";
    monta_titulo($titulo_modulo, '');
?>

<style type="">
    @media print {
        .notprint {
            display: none;
        }
        .div_rolagem{
            display: none;
        }
        .div_rol{
            display: none;
        }
    }

    @media screen {
        .notscreen {
            display: none;
        }
        .div_rol{
            display: none;
        }
    }
    .div_rolagem{
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }
    .div_rol{
        overflow-x: auto;
        overflow-y: auto;
        height: 50px;
    }
</style>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script language="javascript" type="text/javascript">
    
    function gerarRelatorio(tipo){
        
        if(tipo == 'X'){
            $('#tipo_relatorio').val('XLS');
            
            $('#formulario').submit();
        }else{
            $('#tipo_relatorio').val('HTM');
        
            divCarregando();
            jQuery.ajax({
                type: 'POST',
                url: 'par.php?modulo=relatorio/relatorioMaisMedicos&acao=A',
                data: jQuery('#formulario').serialize(),
                async: false,
                success: function(data) {
                    jQuery("#div_relatorio").html(data);
                    divCarregado();
                }
            });
        }
    }
    
</script>

<form id="formulario" method="post" name="formulario">
    <input type="hidden" id="requisicao" name="requisicao" value="relatorio"/>
    <input type="hidden" id="tipo_relatorio" name="tipo_relatorio" value=""/>
    
    <div id="pesquisa" class="notprint">
        <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
            <tr>
                <td class="subtituloDireita">Programa:</td>
                <td>
                    <?PHP 
                        $sql = array(
                            array("codigo"=>228, "descricao"=>'Mais Médico'),
                            array("codigo"=>251, "descricao"=>'Novo Edital 2015')
                        ); 
                        $db->monta_combo('prgid', $sql, 'S', 'Selecione...', '', '', '', '372', 'S', '', '', $prgid); 
                    ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Estado Workflow:</td>
                <td>
                    <?php $sql = "SELECT esdid AS codigo, esddsc AS descricao FROM workflow.estadodocumento WHERE tpdid = " . WF_TPDID_MAIS_MEDICOS . " ORDER BY esddsc"; ?>
                    <?php $db->monta_combo('esdid', $sql, 'S', 'Selecione...', '', '', '', '372', 'N', '', '', $esdid); ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">UF:</td>
                <td>
                    <?php $sql = "SELECT estuf AS codigo, estuf AS descricao FROM territorios.estado ORDER BY estuf"; ?>
                    <?php $db->monta_combo('estuf', $sql, 'S', 'Selecione...', '', '', '', '372', 'N', '', '', $estuf); ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Município:</td>
                <td><?php echo campo_texto('mundescricao', 'N', 'S', 'Município', '50', '150', '', '', '', '', '', 'id="mundescricao"'); ?></td>
            </tr>
            <tr>
                <td class="subtituloDireita">Parceria (Nº Leitos):</td>
                <td>
                    <?php $aryParceria = array(array('codigo' => 'S', 'descricao' => 'Sim'), array('codigo' => 'N', 'descricao' => 'Não')); ?>
                    <?php $db->monta_combo('parceria', $aryParceria, 'S', 'Selecione...', '', '', '', '372', 'N', '', '', $parceria); ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Adesão (Termo de Residência):</td>
                <td>
                    <?php $aryAdesao = array(array('codigo' => 't', 'descricao' => 'Sim'), array('codigo' => 'f', 'descricao' => 'Não')); ?>
                    <?php $db->monta_combo('adesao', $aryAdesao, 'S', 'Selecione...', '', '', '', '372', 'N', '', '', $adesao); ?>
                </td>
            </tr>
            <tr>
                <td align="center" bgcolor="#CCCCCC" colspan="2">
                    <input type="button" name="relatorioHtml" id="relatorioHtml" value="Pesquisar" onclick="gerarRelatorio('H');"/>
                    <input type="button" name="relatorioImpr" id="relatorioImpr" value="Imprimir" onclick="javascript: window.print();">
                    <input type="button" name="relatorioXLS" id="relatorioXLS" value="Gerar Arquivo XLS" onclick="gerarRelatorio('X');"/>
                </td>
            </tr>
        </table>
    </div>
</form>

<br>

<div id="div_relatorio"> <?PHP gerarRelatorio(); ?> </div>
