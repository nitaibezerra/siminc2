<?php 

if( $_REQUEST['carrega'] && $_REQUEST['id'] ){
	$dadosU = carregaDadosUnidadePorUreid( $_REQUEST['id'] );
	echo $dadosU['ungcod'].'|'.$dadosU['ureordenador'].'|'.$dadosU['ureordenadorsub'].'|'.$dadosU['limite'].'|'.$dadosU['saldo'];
	exit;
}

if( $_REQUEST['id'] ){
	$sqlUnidades = carregaDadosUnidade( $_REQUEST['id'] );
	$arrResp = carregaDadosPregao( $_REQUEST['id'] );
}

if( $_POST["submeter"] == 'excluir' && $_POST["ureid"] ) {
	
	$existe_ev = $db->pegaUm("SELECT eveid FROM evento.evento WHERE ureid='".$_POST["ureid"]."'");
	if($existe_ev) die("<script>alert('Unidade de Recurso não pode ser excluído. Existem eventos vinculadas a unidade de recurso.');window.location='evento.php?modulo=principal/cadUnidadeRecurso&acao=A&id=".$_REQUEST["id"]."';</script>");

	$existe_cc = $db->pegaUm("SELECT uccid FROM evento.unidadecontacorrente WHERE ureidpai='".$_POST["ureid"]."' OR ureidfilho='".$_POST["ureid"]."'");
	if($existe_cc) die("<script>alert('Unidade de Recurso não pode ser excluído. Existem movimentações na conta corrente da unidade de recurso.');window.location='evento.php?modulo=principal/cadUnidadeRecurso&acao=A&id=".$_REQUEST["id"]."';</script>");
	
	$sql = "DELETE FROM evento.itemevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.capacidadehospedagem WHERE hoeid IN(SELECT hoeid FROM evento.hotelevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"]."))";
	$db->executar($sql);
	$sql = "DELETE FROM evento.hotelevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.avaliacaoevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.avaliacaosubjetivaevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.anexoevento WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.unidadeparceira WHERE eveid IN (SELECT eveid FROM evento.evento WHERE ureid = ".$_POST["ureid"].")";
	$db->executar($sql);
	$sql = "DELETE FROM evento.evento WHERE ureid = ".$_POST["ureid"];
	$db->executar($sql);
	$sql = "DELETE FROM evento.unidaderecurso WHERE ureid = ".$_POST["ureid"];
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('principal/cadUnidadeRecurso','&id='.$_POST['preid']);
	} else {
		$db->insucesso( 'Operação não realizada com sucesso', '&id='.$_POST['preid'], 'principal/cadUnidadeRecurso'  );
	}
}
	
if( $_POST["submeter"] == 'salvar' ) {
	
	$sql = "select 
				count(*) 
			from 
				evento.unidaderecurso 
			where 
				ungcod = '".$_POST['ungcod']."'";
	
	$boExiste = $db->pegaUm($sql);
	
	if(!$boExiste || $_POST["ureid"]){
	
		$vrRec  = str_replace( '.', '', $_POST['limite'] );
		$limite  = str_replace( ',', '.', $vrRec);
		
		$vrRec  = str_replace( '.', '', $_POST['saldo'] );
		$saldo  = str_replace( ',', '.', $vrRec);
		
		if( !$_POST["ureid"] || $_POST["ureid"] == "" )
		{
			
			$sql = "INSERT INTO evento.unidaderecurso(
		            			preid, 
		            			ungcod,
		            			ureordenador,
		            			ureordenadorsub,
		            			urevalorrecurso,
		            			urevalorsaldo)
		   			 
		            VALUES ('".$_POST["preid"]."', 
		   			 		'".$_POST['ungcod']."', 
		   			 		'".$_POST['ureordenador']."',
		   			 		'".$_POST['ureordenadorsub']."',
		   			 		'".$limite."',
		   			 		'".$saldo."'
		   			 		)";
			//ver($sql,d);
			if(!empty($_POST["preid"]) && !empty($_POST['ungcod']) && !empty($limite)){
				$db->executar($sql);
			}
		}
		else
		{
			$sql = "UPDATE evento.unidaderecurso SET
		            			ungcod = '".$_POST['ungcod']."',
		            			ureordenador = '".$_POST['ureordenador']."',
		            			ureordenadorsub = '".$_POST['ureordenadorsub']."',
		            			urevalorrecurso = '".$limite."',
		            			urevalorsaldo = '".$saldo."'
		            		WHERE
		            			ureid = ".$_POST['ureid'];
			
			$db->executar($sql);
			
		}
		if(!empty($_POST["preid"]) && !empty($_POST['ungcod']) && !empty($limite)){
			if($db->commit()){
				$db->sucesso('principal/cadUnidadeRecurso','&id='.$_POST['preid']);
			} else {
				$db->insucesso( 'Operação não realizada com sucesso', '', 'principal/cadUnidadeRecurso'  );
			}
		}
		
	}else{
		
		alert('Esta unidade já esta cadastrada no sistema!');
		
	}
	
}
?>
<head>
   <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
    <script src="../includes/entidades.js"></script>
    <script src="../includes/calendario.js"></script>
    
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
</head>

<?php 
$titulo_modulo = "Unidades de Recursos";
//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';
monta_titulo( $titulo_modulo, 'Cadastro das Unidades de Recursos' );
echo'<br>';

?>	
<form name ="formulario" action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario">
    <input type="hidden" name="submeter" id="submeter" value="">
    <input type="hidden" name="preid" id="preid" value="<?php echo $_REQUEST['id'] ?>">
    <input type="hidden" name="ureid" id="ureid" value="">
    <input type="hidden" name="limiteantigo" id="limiteantigo" value="">
    <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
           <td class ="SubTituloDireita" align="right">Código: 
           </td>
           <td> 
                <?= campo_texto('precodpregao', 'N', 'N', '', 8, 7, '', '', 'left', '',  0, 'id="precodpregao" onblur="MouseBlur(this);"', '', $arrResp['precodpregao'] ); ?>               
                <a href="#" onclick="consultarPregao()"><img src="../imagens/preview.gif" align="top" id="pesquisar" title="Consultar Pregão" style="cursor:pointer;" border="0"/></a>
           </td>
        </tr>
        <tr>
           <td class ="SubTituloDireita" align="right">Número Processo: 
	            <input type="hidden" name="numero" id="numero" value="0" />
           </td>
           <td> 
                <?= campo_texto('prenumprocesso', 'N', 'N', '', 24, 20, '', '', 'left', '',  0, 'id="prenumprocesso" onblur="MouseBlur(this);"', '', $arrResp['prenumprocesso'] ); ?>                
           </td>
        </tr>
        <tr>
            <td class ="SubTituloDireita" align="right">Valor Contratado: </td>
            <td> 
             	<?= campo_texto('prevalorcontratado', 'N', 'N', '', 20, 20, '###.###.###.###,##', '', 'left', '',  0, 'id="prevalorcontratado" onblur="MouseBlur(this);"','', number_format($arrResp["prevalorcontratado"],2,",",".") ); ?>
            </td>
        </tr>
	</table>
<?php echo'<br>'?>
	<table class="listagem" width="95%" align="center" border="0" cellpadding="2" cellspacing="0">
		<thead>
		<tr>
			<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('1','ASC');" title="Ordenar por Unidade" valign="top" align="center" bgcolor="#e9e9e9">
				<strong>Inserir Unidade</strong>
			</td>
			<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('2','ASC');" title="Ordenar por Limite" valign="top" align="center" bgcolor="#e9e9e9">
				<strong>Ordenador de Despesa</strong>
			</td>
			<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('2','ASC');" title="Ordenar por Limite" valign="top" align="center" bgcolor="#e9e9e9">
				<strong>Ordenador de Despesa Substituto</strong>
			</td>
			<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('2','ASC');" title="Ordenar por Limite" valign="top" align="center" bgcolor="#e9e9e9">
				<strong>Inserir Limite</strong>
			</td>
			<td class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('2','ASC');" title="Ordenar por Limite" valign="top" align="center" bgcolor="#e9e9e9">
				<strong>Inserir Saldo</strong>
			</td>
			<td class="title" width="8%" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(229, 234, 237); border-left: 1px solid rgb(255, 255, 255);" onclick="ordena('2','ASC');" title="Ordenar por Limite" valign="top" align="center" bgcolor="#e9e9e9">
			</td>
		</tr> 
		</thead>
		<tr>
			<td valign="top" align="center" bgColor="#e9e9e9" class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<?php 
				$sql = "SELECT ungcod as codigo, ungdsc as descricao FROM public.unidadegestora WHERE ungstatus = 'A' order by descricao";
				$db->monta_combo('ungcod', $sql, 'S', "Selecione...", '', '', '', '', 'S', 'ungcodid'); ?>				
			</td>
			<td valign="top" align="center" bgColor="#e9e9e9"class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<?php echo campo_texto('ureordenador', 'S', 'S', '', 32, 200, '', '', 'left', '',  0, 'id="ureordenador" onblur="MouseBlur(this);"' ); ?>
			</td>
			<td valign="top" align="center" bgColor="#e9e9e9"class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<?php echo campo_texto('ureordenadorsub', 'S', 'S', '', 32, 200, '', '', 'left', '',  0, 'id="ureordenadorsub" onblur="MouseBlur(this);"' ); ?>
			</td>
			<td valign="top" align="center" bgColor="#e9e9e9"class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<?php echo campo_texto('limite', 'S', 'S', '', 22, 20, '###.###.###.###,##', '', 'left', '',  0, 'id="limiteid" onblur="MouseBlur(this);"' ); ?>
			</td>
			<td valign="top" align="center" bgColor="#e9e9e9"class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<?php echo campo_texto('saldo', 'S', 'S', '', 22, 20, '###.###.###.###,##', '', 'left', '',  0, 'id="saldoid" onblur="MouseBlur(this);"' ); ?>
			</td>
			<td valign="top" align="center" bgColor="#e9e9e9" class="title" style="border-right: 1px solid rgb(192, 192, 192); border-bottom: 1px solid rgb(192, 192, 192); border-left: 1px solid rgb(255, 255, 255);">
				<input type="button" value="Salvar" id="pgSalvar" name="pgSalvar" onclick="salvar();"/> 
			</td>
		</tr>
	</table>
<?php
if( $_REQUEST['id'] ){

	$cabecalho = array("Ações","Unidades","Ordenador de Despesa","Ordenador de Despesa Substituto","Limites","Saldo");	
	$db->monta_lista_grupo( $sqlUnidades, $cabecalho, 25, 10, 'S', 'center', 'N');

	$sql = "SELECT SUM(urevalorrecurso) FROM evento.unidaderecurso WHERE preid = ".$_REQUEST['id'];
	$total = $db->pegaUm( $sql );
	echo '<input type="hidden" name="totalSomado" id="totalSomado" value="'.$total.'">';
}
?>
</form>
<script type="text/javascript">

function exibirExtrato( ureid )
{	
	var janela = window.open("evento.php?modulo=principal/listaExtratoUnidade&acao=A&ureid=" + ureid, "prcEscola", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=400");
	janela.focus();
}

function validaFormulario() {

	var prenumprocesso 		= document.getElementById('prenumprocesso').value; 
	var precodpregao		= document.getElementById('precodpregao').value; 
	var predescpregao		= document.getElementById('predescpregao').value; 
	var datainiciovig		= document.getElementById('preiniciovig'); 
	var datafimvig 			= document.getElementById('prefimvig'); 
	var prevalorcontratado	= document.getElementById('prevalorcontratado').value; 
	var prevalorempenhado	= document.getElementById('prevalorempenhado').value; 

	if( prenumprocesso == '')
	{
		alert('O Campo "Número do Processo" é Obrigatório');
		return false;
	}
	if( precodpregao == '')
	{
		alert('O Campo "Código do Pregão" é Obrigatório');
		return false;
	}
	if( predescpregao == '')
	{
		alert('O Campo "Descrição do Pregão" é Obrigatório');
		return false;
	}
	if( datainiciovig == '')
	{
		alert('O Campo "Início da Vigência do Pregão" é Obrigatório');
		return false;
	}
	if( datafimvig == '')
	{
		alert('O Campo "Fim da Vigência do Pregão" é Obrigatório');
		return false;
	}
	if( prevalorcontratado == '')
	{
		alert('O Campo "Valor Contratado do Pregão" é Obrigatório');
		return false;
	}
	if( prevalorempenhado == '')
	{
		alert('O Campo "Valor Empenhado do Pregão" é Obrigatório');
		return false;
	}
	if(!validaDataMaior(datainiciovig, datafimvig))
	{
        alert("A data de 'Início da Vigência' não pode ser maior do que a data do'Fim da Vigência'.");
        datafimvig.focus();
        datafimvig.value = ''; 
        return false;
	}

	document.getElementById('submeter').value = 'salvar';
	document.getElementById('formulario').submit();
	
}

function salvar()
{
	if(document.getElementById('ungcodid').value == ''){
		alert('O campo unidade é obrigatório!');
		document.getElementById('ungcodid').focus();
		return false;
	}

	if(document.getElementById('ureordenador').value == ''){
		alert('O campo Ordenador de Despesa é obrigatório!');
		document.getElementById('ureordenador').focus();
		return false;
	}
	if(document.getElementById('ureordenadorsub').value == ''){
		alert('O campo Ordenador de Despesa Substituto é obrigatório!');
		document.getElementById('ureordenadorsub').focus();
		return false;
	}

	if(document.getElementById('limiteid').value == ''){
		alert('O campo limite é obrigatório!');
		document.getElementById('limiteid').focus();
		return false;
	}
	
	if(document.getElementById('saldoid').value == ''){
		alert('O campo saldo é obrigatório!');
		document.getElementById('saldoid').focus();
		return false;
	}
	
	total = parseFloat(document.getElementById('totalSomado').value);
	contratado = parseFloat(replaceAll(replaceAll(document.getElementById('prevalorcontratado').value,".",""),",","."));
	limite = parseFloat(replaceAll(replaceAll(document.getElementById('limiteid').value,".",""),",","."));
	
	if( document.getElementById('ureid').value != '' ){ //quando for alterar!
		menos = parseFloat(replaceAll(replaceAll(document.getElementById('limiteantigo').value,".",""),",","."));
		total = parseFloat(total) - parseFloat(menos);
	}

	soma = parseFloat(total) + parseFloat(limite);
	
	if( soma > contratado ){
		alert('A soma dos valores do Limite não pode ser maior que o valor contratado');
	} else {
		document.getElementById('submeter').value = 'salvar';
		document.getElementById('formulario').submit();
	}
}

function consultarPregao() {
	window.open('evento.php?modulo=principal/popConsultaPregao&acao=A','page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,width=400,height=500');
}

function conferirDigitoVerificador() {
	var txtDigitado = campoNumeroProcessoSidoc;
	var atual       = "";
	var conta       = 14;
	var conta2      = 0;
	var numero      = "";
	var valor       = 0;
	var digitoVerif = "";
	var digito1     = "";
	var digito2     = "";
	var ano         = "";
	
	txtDigitado = txtDigitado.substr(12,17);
	digitoVerif = txtDigitado.substr(15,2);
	numero = txtDigitado.substr(0,15);
	conta2 = 1;
	for( conta=14 ; conta >= 0 ; conta-- ) {
		var conta2 = conta2 + 1;
		var algarismo = numero.substr(conta,1);
		valor = valor + eval(algarismo) * conta2;
	}
	if( valor%11 == 0 ) {
		digito1 = "1";
	}
	else if( valor%11 == 1 ) {
		digito1 = "0";
	}
	else {
		digito1 = "" + (11-(valor%11));
	}
		numero = numero + digito1;
	valor = 0;
	conta2 = 1;	
		for( conta=15 ; conta >= 0 ; conta-- ) {
		var conta2 = conta2 + 1;
		var algarismo = numero.substr(conta,1);
		valor = valor + eval(algarismo) * conta2;
	}
	if( valor%11 == 0 ) {
		digito2 = "1";
	}
	else if( valor%11 == 1 ) {
		digito2 = "0";
	}
	else {
		digito2 = "" + (11-(valor%11));
	}
		if(numero){
		var result = digitoVerif == (digito1 + digito2);
	}else{
		var result = true;
	}
		alert(result);
	if( result ) {
		return true;
	}
	else {
		var mensagemDeErro = "Digito verificador não confere.";
		return false;
	}
}

function alterar(id)
{
	var myAjax = new Ajax.Request('evento.php?modulo=principal/cadUnidadeRecurso&acao=A', {
        method:     'post',
        parameters:  "carrega=true&id=" + id,
        onComplete: function (res){	
        	if(res.responseText){
        		dados = res.responseText;
   			 	dados = dados.split('|');
        		document.getElementById('ungcodid').value = dados[0];
        		document.getElementById('ureordenador').value = dados[1];
        		document.getElementById('ureordenadorsub').value = dados[2];
        		limite = mascaraglobal('#.###.###.###,##', dados[3]);
        		saldo = mascaraglobal('#.###.###.###,##', dados[4]);
        		document.getElementById('limiteid').value = limite;
        		document.getElementById('saldoid').value = saldo;
        		document.getElementById('limiteantigo').value = limite;
        		document.getElementById('ureid').value = id;

        		var select = new Array();
				select[0] = 'ungcodid'; 
				
				var i = 0;
				while( i <= select.length ){
					var elemento = document.getElementById(select[i]);
					for (a=0; a < elemento.options.length; a++){
						if (dados[i] == elemento.options[a].value){
							elemento.selectedIndex = a;
							continue;	
						}
					}
					i++;
				}
        
        	}
	}	
  });
}

function excluir(id)
{
	if( confirm("Deseja excluir este item?") )
	{
		var formulario	= 	document.getElementById('formulario');
		document.getElementById('submeter').value = 'excluir';
		document.getElementById('ureid').value = id;
		
		formulario.submit();
	} else {
		return false;
	}
}
</script>