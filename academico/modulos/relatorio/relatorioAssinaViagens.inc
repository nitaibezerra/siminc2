<?php

if ($_POST){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioViagensResultado.inc");
	exit;
}

	
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Autorização de Viagens para o Exterior', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
function gerarRelatorio(req){
	var formulario = document.formulario;

	document.getElementById('req').value = req;
	
	selectAllOptions( formulario.ano );
	selectAllOptions( formulario.unidade );
	selectAllOptions( formulario.situacao );
			
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	

	formulario.submit();
	
	janela.focus();
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

</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">
<input type="hidden" name="req" id="req" value="" />	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<?php
			
				// ano
				$anoini = 2011;
				$anofim = date('Y');
				$arrAno = array();
				$stSql = "SELECT $anoini as codigo, $anoini as descricao";
				for($anoini+1; $anoini<=$anofim; $anoini++){
					$stSql .= " UNION
								SELECT $anoini as codigo, $anoini as descricao
							  ";
				}
				$stSql .= " ORDER BY 1 ";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Ano', 'ano',  $stSql, $stSqlCarregados, 'Selecione o(s) Ano(s)' ); 

				
				// unidades
				$stSql = " select
								ent.entid as codigo,
								ent.entnome as descricao
							from 
								academico.autviagemexterior tbl1
							inner join 
								entidade.entidade ent ON tbl1.entid = ent.entid
							where 
								avestatus = 'A'
							group by 
								ent.entid, ent.entnome
							order by
								entnome ";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Unidade', 'unidade',  $stSql, $stSqlCarregados, 'Selecione a(s) Unidade(s)' ); 
				
				// Situação
				/*
				$sql = "SELECT esdid as codigo, esddsc as descricao
						FROM workflow.estadodocumento e
						INNER JOIN workflow.tipodocumento t on t.tpdid = e.tpdid
						WHERE esdstatus = 'A' and sisid = ".$_SESSION['sisid']."
						ORDER BY esddsc";
				*/
				$sql = "select	
							ed.esdid as codigo,
							ed.esddsc as descricao
						from 
							academico.autviagemexterior tbl1
						inner join 
							workflow.documento d ON d.docid = tbl1.docid
						inner join 
							workflow.estadodocumento ed on ed.esdid = d.esdid
						where 
							avestatus = 'A'
						group by 
							ed.esdid, ed.esddsc
						order by
							ed.esddsc";
				
				$stSqlCarregados = "";
				mostrarComboPopup( 'Situação', 'situacao',  $sql, $stSqlCarregados, 'Selecione a(s) Situação(ões)' ); 
				
			?>
	<tr>
		<td width="40%" align="left">
			&nbsp;
		</td>
		<td align="left">
			<input type="button" name="Gerar" value="Gerar" onclick="javascript:gerarRelatorio(1);"/>
		</td>
	</tr>
</table>
</form>
</body>
</html>