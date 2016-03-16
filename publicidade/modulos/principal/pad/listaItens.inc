<?php
// monta cabeÃ§alho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';

// incluindo as classes necessÃ¡rias
$oPad = new Pad();
$oItemPad = new ItemPad();
$oFornecedor = new Fornecedor();
$oTipoServico = new TipoServico();
$oTipoPeca = new TipoPeca();
$oContratoHonorario = new ContratoHonorario();

// monta o tÃ­tulo da tela
monta_titulo('Lista Itens da PAD', '');
?>
<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script type="text/javascript">
    jQuery(document).ready(function() {
        jQuery('#btnPesquisar').click(function() {
            jQuery('#requisicao').val('listar');
            jQuery("#formularioPesquisaItens").submit();
        });
    });

</script>
<form name="formularioPesquisaItens" id="formularioPesquisaItens" method="post">
    <table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
        <tr>
            <td class="SubtituloDireita">Fornecedor:</td>
            <td class="campo">
                <div id='fornecedor' style='display: inline'>
                    <?php $forid = $_REQUEST['forid_fornecedor']; $oFornecedor->monta_combo_fornecedor($forid, 'S', '', 'cadItemPad'); ?>
                    <script>
                        var ag = document.getElementById('forid');
                        var opt = ag.getElementsByTagName('option');

                        for (var i = 0; i < opt.length; i++) {
                            if (opt[0].value == '') {
                                opt[0].value = 0;
                            }
                        }
                    </script>
                </div> 
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo de Servi&ccedil;o:</td>
            <td class="campo"><?php
            $tseid = $_REQUEST['tseid'];
                if ($readOnly == 'S') {
                   $oTipoServico->monta_combo_tipo_servico($tseid, 'S');
                } else {
                    $oTipoServico->monta_combo_tipo_servico($tseid, 'S', 'listaItens');
                }
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Categoria:</td>
            <td class="campo"><div id='categoria'>
                    <?php $oTipoServico->monta_combo_categoria('', '', 'S'); ?>
                </div></td>
        </tr>
        <tr>
            <td class="SubtituloDireita">Tipo de Pe&ccedil;a Publicit&aacute;ria:
            </td>
            <td class="campo"><?php $oTipoPeca->monta_combo_tipo_peca($tppid, 'S'); ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita"> Exercicío: </td>
            <td class="campo">
                <select name="exercicio" id="exercicio">
                     <option value="">Selecione</option>
                    <option value="2015" <?php if($_REQUEST['exercicio'] == '2015'){echo 'selected';}?>>2015</option>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita"> Valor do Item: </td>
            <td class="campo">
                <?= campo_texto('padvaloritem', 'N', 'S', 'Informe o valor total do Item', 16, 16, '#.###.###.###,##', '', 'left', '', 0, 'id="padvaloritem"','',$_REQUEST['padvaloritem']); ?>
            </td>
        </tr>
        <tr>
            <td class="SubtituloDireita"> Nº OB: </td>
            <td class="campo">
                <?= campo_texto('numOB', 'N', 'S', 'Informe o numero OB', 16, 16, '', '', 'left', '', 0, 'id="numOB"','',$_REQUEST['numOB']); ?>
            </td>
        </tr>

        <tr class="buttons">
            <td colspan='2' align='center'>
                <input type='hidden' name='requisicao' id='requisicao' />
                <input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">

            </td>
        </tr>
    </table>
</form>
<?php
if ($_REQUEST['requisicao'] == 'listar') {
    if ($_REQUEST['padvaloritem']) {
        $valor = str_replace(',', '.', str_replace('.', '', $_REQUEST['padvaloritem']));
        $where .= " AND item.ipavalortotal = '{$valor}' ";
    }
    if ($_REQUEST['tseid']) {
        $where .= " AND item.tseid = '{$_REQUEST['tseid']}' ";
    }
    if ($_REQUEST['forid'] || $_REQUEST['forid_fornecedor']) {
        if ($_REQUEST['forid']) {
            $fornecedor = $_REQUEST['forid'];
        }if ($_REQUEST['forid_fornecedor']) {
            $fornecedor = $_REQUEST['forid_fornecedor'];
        }
        $where .= " AND item.forid = '{$fornecedor}' ";
    }

    if ($_REQUEST['catid']) {
        $where .= " AND item.catid = '{$_REQUEST['catid']}' ";
    }

    if ($_REQUEST['tppid']) {
        $where .= " AND item.tppid = '{$_REQUEST['tppid']}' ";
    }

    if ($_REQUEST['exercicio']) {
        $where .= " AND  substr(item.ipanumitempad, 0, 5) = '{$_REQUEST['exercicio']}' ";
    }
    
    if ($_REQUEST['numOB']) {
        $where .= " AND  pipnumordembancaria = '{$_REQUEST['numOB']}' ";
    }


    $sql = "SELECT 
	
		              CASE WHEN item.ipastatus = 'A' THEN 'Aprovado' 
					      ELSE 
					  	  CASE WHEN item.ipastatus = 'C' THEN 'Cancelado'
					  	      ELSE
					  		  CASE WHEN item.ipastatus = 'F' THEN 'Faturado'
							      ELSE
								  CASE WHEN item.ipastatus = 'P' THEN 'Pago'
								      ELSE
									  CASE WHEN item.ipastatus = 'G' THEN 'Glosado'
									  END
							      END
					  		  END
					      END	
		              END AS situacao,
		              CASE 
						    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 10 ) THEN
							
						        substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '000' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
						    ELSE
							CASE 
							    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 100 ) THEN
						
								substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '00' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
							    ELSE
								CASE 
								    WHEN ( CAST( substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad) )AS int) < 1000 ) THEN
						
									substr( ipanumitempad, 0, strpos( ipanumitempad, '.') + 1 ) || '0' || substr( ipanumitempad, strpos( ipanumitempad, '.') + 1 ,  char_length(ipanumitempad))
									
								END
							END
							
						END as ipanumitempad,
		              fornec.fornome,
		              tipo.tsedsc,
		              item.ipaqtd||' ',
		              item.ipavalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.glosaitempad glo WHERE glo.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(glosa.gipvalor AS text) END AS gipvalor,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(fatura.fipdataftura,'DD/MM/YYYY') END AS fipdataftura,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE fatura.fipnumfaturaagencia||' ' END AS fipnumfaturaagencia,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.faturamentoitempad fat WHERE fat.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(fatura.fipvalortotal AS text) END AS fipvalortotal,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE TO_CHAR(pag.pipdatapagamento,'DD/MM/YYYY') END AS pipdatapagamento,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipnumordembancaria||' ' AS text) END AS pipnumordembancaria,
		              
		              CASE WHEN (SELECT COUNT(*) FROM publicidade.pagamentoitempad pag WHERE pag.ipaid=item.ipaid) < 1 THEN '-' ELSE CAST(pag.pipvalorfaturado AS text) END AS pipvalorfaturado
		              
	                  FROM publicidade.itenspad item
	                  JOIN publicidade.pad pad ON item.padid=pad.padid
	                  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid 
	                  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	                  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid and fatura.fipstatus = 'A'
	                  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	                  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid and pag.pipstatus = 'A'
	                   WHERE substr(item.ipanumitempad, 0, 5) >= '2015' {$where}
	                  ORDER BY ipanumitempad ";
                           
                           
    $cabecalho = array('Status', 'N&uacute;mero', 'Fornecedor', 'Tipo de Servi&ccedil;o', 'Qtd.', 'Valor Total', 'Valor Glosado', 'Dt. Faturamento', 'N&ordm; Nota', 'Valor Faturado', 'Dt. Pagamento', 'N&ordm; da OB', 'Valor Pago');
    $db->monta_lista($sql, $cabecalho, 10, 50, false, "center", 'S', 'formularioItens', '', array('left', 'center', 'right'));
}
?>
