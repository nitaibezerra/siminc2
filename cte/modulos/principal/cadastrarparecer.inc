<?php
/************************* VERIFICA USUÁRIO ******************************************/
cte_verificaSessao();

/************************* INCLUDES *************************************************/
include APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'cte/classes/ParecerPar.class.inc';
echo '<br />';

/************************* DECLARAÇÃO DE VARIAVEIS *********************************/
$inuid 	= $_SESSION['inuid'];
$cpf 	= $_SESSION['usucpf'];
$texto 	= $_REQUEST['partexto'];

if ($_REQUEST['acao'] == 'A'){
    $tppid = 1;
}elseif ($_REQUEST['acao'] == 'B'){
    $tppid = 2;
}elseif ($_REQUEST['acao'] == 'C'){
    $tppid = 3;
}

/************************* INSTANCIA CLASSES E OBJETOS *****************************/
$obParecer = new ParecerPar();

/************************* CARREGA DADOS PARA MOSTRAR NA TELA *********************/
if( $tppid && $inuid  ){ // se o monitoramento do item já existe carrega dados.
	$dadosParecer = $obParecer->recuperaDadosPorID($inuid, $tppid);
	if($dadosParecer){
		$dadosParecer 					= current($dadosParecer);
		$dadosParecer['parhistorico'] 	= $dadosParecer['parhistorico'] == NULL ? '1' : $dadosParecer['parhistorico']; 
		$texto 							= $dadosParecer['partexto'];
		$pardata						= $dadosParecer['pardata'];
	}
}

/************************* SALVA DADOS ********************************************/
if($_REQUEST['partexto'] && $_REQUEST['salvar']){ // INSERE
	if($_REQUEST['partexto']){
		$texto = $_REQUEST['partexto'];
	}
	$parHistorico = $obParecer->recuperaMaiorIdHistorico($inuid, $tppid);
	$obParecer->inuid 			= $inuid;
	$obParecer->usucpf 			= $cpf;
	$obParecer->tppid 			= $tppid;
	$obParecer->pardata 		= date('Y-m-d');
	$obParecer->partexto 		= $texto;
	$obParecer->parhistorico 	= $parHistorico;
	$obParecer->salvar();
	$obParecer->commit();
	 echo "<script type=\"text/javascript\">"
            ."alert('Novo parecer salvo com sucesso.');"
            ."window.location.href='cte.php?modulo=principal/estrutura_avaliacao&acao=A';"
            ."</script>\n";
}

/************************* CARREGA LISTAS DE HISTÓRICOS *********************/
if($tppid == 1){
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
}

if($tppid == 2){
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
}

cte_montaTitulo('Cadastro de parecer', $titulo_modulo);

?>
<form method="POST"  name="formulario">
<input type=hidden name="salvar" id="salvar" value="0" />
<input type=hidden name="pardata" id="pardata" value="<?php echo $pardata; ?>" />
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
    	<td width="25%" align='right' class="SubTituloDireita">Tipo de parecer:</td>
        <td><?php echo $titulo_modulo;?></td>
    </tr>
    <tr>
       	<td align='right' class="SubTituloDireita">Parecer:</td>
        <td><?php echo campo_textarea('partexto', 'S', 'S', 'Parecer', '100', '10', null ,'', 0, '', false, 'Parecer');?></td>
    </tr>
    <tr bgcolor="#C0C0C0">
       	<td colspan="2" align="center">
        	<input type="button" name="salvar" value="salvar"  onclick="salvarDados();" />
        </td>
    </tr>
</table>
</form>
<?php if($tppid == 1){?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Pareceres Financeiros</th>
		</tr>
		<tr>
			<?php $db->monta_lista($arrayFinanceiro,$cabecalhoFinanceiro,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
<?php }else if($tppid == 2){ ?>
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" width="100%">
		<tr>
			<th class="tdDegradde02" colspan="3" >Lista de Histórico de Pareceres de engenharia</th>
		</tr>
		<tr>
			<?php $db->monta_lista($arrayEngenharia,$cabecalhoEngenharia,20, 10, 'N', '', ''); ?>
		</tr>
	</table>
<?php } ?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript">
	function salvarDados(){
		var nomeform 		= 'formulario';
		campos 				= new Array("partexto");
		tiposDeCampos 		= new Array("textarea");
		if(validaForm(nomeform, campos, tiposDeCampos, false )){
			document.getElementById("salvar").value = 1;
			document.formulario.submit();
		}
	}

	function mostraParecerFinaceiro(parid){
		var janela = window.open( '/cte/cte.php?modulo=principal/visualizaParecerEngenFinanceiro&acao=A&parid='+parid,'blank','height=400,width=700,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
		janela.focus();
	}
	
	function mostraParecerEngenharia(parid){
		var janela = window.open( '/cte/cte.php?modulo=principal/visualizaParecerEngenFinanceiro&acao=B&parid='+parid,'blank','height=400,width=700,status=yes,toolbar=no,menubar=yes,scrollbars=yes,location=no,resizable=yes');
		janela.focus();
	}
</script>

