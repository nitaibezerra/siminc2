<?php

if($_REQUEST['exporta'] == "true"){
	
	$ids = implode(',',$_REQUEST['itens']);
	
	$sql = "select 
				a._atinumero,  
				icldsc, 
				iclprazo as dt_inicio, 
				iclprazo as dt_fim, 
				null as cpf_executor, 
				evidencia_execucao, 
				null as cpf_validador, 
				evidencia_validacao, 
				null as cpf_certificador, 
				evidencia_certificador, 
				iclcritico 
		from pde.atividade a 
        inner join pde.itemchecklist icl ON icl.atiid = a.atiid 
        inner join workflow.documento d ON d.docid = icl.docid
        left join ( select iclid, etcevidencia as evidencia_execucao from pde.etapascontrole where tpvid = 1 ) ec1 ON ec1.iclid = icl.iclid
        left join ( select iclid, etcevidencia as evidencia_validacao from pde.etapascontrole where tpvid = 2 ) ec2 ON ec2.iclid = icl.iclid
        left join ( select iclid, etcevidencia as evidencia_certificador from pde.etapascontrole where tpvid = 3 ) ec3 ON ec3.iclid = icl.iclid
        where _atiprojeto = 114098 
        and atistatus = 'A'
        ".(!empty($ids) ? 'and a.atiid in ('.$ids.')' : '')." 
        
        and SUBSTRING(_atinumero FROM 0 FOR 3) = '1.' 
        and atitipoenem is not null 
        order by icl.iclid";
	//die($sql);
	$html = array();
	$rs = $db->carregar($sql);
	
	if($rs){
		foreach($rs as $dados){
			$html[] = $dados['_atinumero']
					.";".$dados['icldsc']
					.";".$dados['dt_inicio']
					.";".$dados['dt_fim']
					.";".$dados['cpf_executor']
					.";".$dados['evidencia_execucao']
					.";".$dados['cpf_validador']
					.";".$dados['evidencia_validacao']
					.";".$dados['cpf_certificador']
					.";".$dados['evidencia_certificador']
					.";".$dados['iclcritico'];
		}
	}
	
	header("Content-type: text/plain");
	//header("Content-Length: $size");
	//header('Content-Disposition: attachment; filename="maladireta.csv"');
	header("Content-Disposition: attachment; filename=SIMEC_".date("His")."_checklistEnem.csv");
	//header("Pragma: no-cache");
	//header("Expires: 0");
	
	$html = implode("\n", $html);
	print $html; 
	exit;

}

// monta cabeçalho 
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$titulo = "Exportar Item checklist";

monta_titulo( $titulo, '&nbsp;');
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
		
	/**
	 * Alterar visibilidade de um bloco.
	 * 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.
	 * 
	 * @param string indica o campo a ser mostrado/escondido
	 * @return void
	 */
	function onOffCampo( campo )
	{
		var div_on = document.getElementById( campo + '_campo_on' );
		var div_off = document.getElementById( campo + '_campo_off' );
		var input = document.getElementById( campo + '_campo_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '1';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '0';
		}
	}
	
	$(function(){
		$('#bt_exportar').click(function(){			
			selectAllOptions( document.getElementById( 'itens' ) );
			//document.location.href = document.location+'&exporta=true';
			$('#exporta_item').val('true');
			$('#exporta_Form').submit();
		});
	});
	
	
</script>
<form id="exporta_Form" method="post" action="">
	<input type="hidden" name="exporta" id="exporta_item" value="" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<?php
		// Região
		$stSql = " select a.atiid as codigo, a._atinumero || ' - ' || a.atidescricao as descricao from pde.atividade a 
	        inner join pde.itemchecklist icl ON icl.atiid = a.atiid 
	        inner join workflow.documento d ON d.docid = icl.docid
	        where _atiprojeto = 114098 
			and atistatus = 'A'			
			and SUBSTRING(_atinumero FROM 0 FOR 3) = '1.' 
			and atitipoenem is not null  
	        order by icl.iclid ";
		
		$stSqlCarregados = "";
		mostrarComboPopup( 'Itens', 'itens', $stSql, $stSqlCarregados, 'Selecione o(s) Item(s)' );
		?>
		<tr>
			<td class="SubTituloDireita">&nbsp;</td>
			<td>
				<input type="button" value="Exportar" id="bt_exportar" style="cursor: pointer;"/>
			</td>		
		</tr>
	</table>
</form>