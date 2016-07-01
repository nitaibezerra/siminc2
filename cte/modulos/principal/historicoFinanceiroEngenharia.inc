<?php
/************************* DECLARAÇÃO DE VARIAVEIS *********************************/
$inuid 	= $_SESSION['inuid'];
$cpf 	= $_SESSION['usucpf'];

/************ LISTA PARECER FINANCEIRO ****************/
	$sqlListaFinanceiro = "	SELECT  '<a style=\"cursor:pointer; margin: 0 -20px 0 20px;\" onclick=\"mostraParecerFinaceiro('|| parid ||');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao, 
									parhistorico,  
									to_char(pardata, 'DD/MM/YYYY') AS pardata, 
									'' as identificacao   
							FROM cte.parecerpar 
							WHERE inuid = ".$inuid." AND tppid = 1
							ORDER BY parhistorico DESC;";	
	$arrayFinanceiro = $db->carregar($sqlListaFinanceiro);
	
	if(is_array($arrayFinanceiro)){ // popula campo identificação
		for($cont = 0; $cont < count($arrayFinanceiro); $cont++){
			$arrayFinanceiro[$cont]['identificacao'] = "Antigo";	
		}
		$arrayFinanceiro[0]['identificacao'] = "Atual"; 
	}else{ // Se não existe pareceres cria array vazio para não dar erro no monta_lista
		$arrayFinanceiro = array();
	}
	$cabecalhoFinanceiro = array("Ação", "N° do Parecer","Data de Criação","Tipo");
	
/************ LISTA PARECER DE ENGENHARIA ****************/
	$sqlListaEngenharia = "	SELECT  '<a style=\"cursor:pointer; margin: 0 -20px 0 20px;\" onclick=\"mostraParecerEngenharia('|| parid ||');\"><img src=\"/imagens/alterar.gif\" border=0 title=\"Selecionar\"></a>' as acao, 
								parhistorico,  to_char(pardata, 'DD/MM/YYYY') AS pardata , '' as identificacao 
							FROM cte.parecerpar 
							WHERE inuid = ".$inuid." AND tppid = 2
							ORDER BY parhistorico DESC;";
	$arrayEngenharia = $db->carregar($sqlListaEngenharia);
	
	if(is_array($arrayEngenharia)){ // popula campo identificação
		for($contE = 0; $contE < count($arrayEngenharia); $contE++){
			$arrayEngenharia[$contE]['identificacao'] = "Antigo";	
		}
		$arrayEngenharia[0]['identificacao'] = "Atual";
	}else{ // Se não existe pareceres cria array vazio para não dar erro no monta_lista
		$arrayEngenharia = array();
	}
	$cabecalhoEngenharia = array("Ação", "N° do Parecer","Data de Criação","Tipo");
	
if (!$arrayEngenharia && !$arrayFinanceiro) {
        echo '<script type="text/javascript">'
            ,'alert("Não há pareceres de Engenharia e Finaceiros lançados até o momento.");'
            ,'window.close();'
            ,'</script>';
}
?>
<html>
<head>
	<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="/includes/listagem.css"/>
</head>
<body>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Pareceres Financeiros</th>
		</tr>
		<tr>
			<?php $db->monta_lista($arrayFinanceiro,$cabecalhoFinanceiro,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
	
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Pareceres de engenharia</th>
		</tr>
		<tr>
			<?php $db->monta_lista($arrayEngenharia,$cabecalhoEngenharia,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
<script type="text/javascript">
	function mostraParecerFinaceiro(parid){
		var janela = window.open( '/cte/cte.php?modulo=principal/visualizaParecerEngenFinanceiro&acao=A&parid='+parid,'_blank','height=400,width=700,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
	}
	
	function mostraParecerEngenharia(parid){
		var janela = window.open( '/cte/cte.php?modulo=principal/visualizaParecerEngenFinanceiro&acao=B&parid='+parid,'_blank','height=400,width=700,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
	}
</script>
</body>
</html>