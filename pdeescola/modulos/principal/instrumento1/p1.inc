<?php
pde_verificaSessao();
/*----------------- Aï¿½ï¿½es - Formulario ---------------------*/ 
function salva (){
	global $db,$pdeid;
	if($_POST["submetido"]) {
		if($_REQUEST["pdetelefone"] == NULL){
			$_REQUEST["pdetelefone"] = "0";
		}
		$sql = "UPDATE pdeescola.pdeescola
						SET 
							pdeuf 			= '".$_REQUEST["pdeuf"]."',
							pdemunicipio 	= '".$_REQUEST["pdemunicipio"]."', 
							pdenome 		= '".substr($_REQUEST["pdenome"],0,100)."',
							pdenomediretor 	= '".substr($_REQUEST["pdenomediretor"],0,100)."',
							pdebairro 		= '".substr($_REQUEST["pdebairro"],0,100)."',
							pdelogradouro 	= '".substr($_REQUEST["pdelogradouro"],0,100)."',
							pdecomplemento 	= '".substr($_REQUEST["pdecomplemento"],0,100)."',
							pdetelefone 	= '".$_REQUEST["pdetelefone"]."',
							pdeemail 		= '".substr($_REQUEST["pdeemail"],0,100)."',
							tloid 			= ".$_REQUEST["area"]."
							
						WHERE
							pdeid = ". $pdeid;
		$db->executar($sql);
		
		$epfid = $db->pegaUm("SELECT
								epfid
							  FROM
							  	pdeescola.estruturaperfilfuncionamento
							  WHERE
							  	trim(epfnomearquivo) = 'p1' AND
							  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
		
		$existe = $db->pegaUm("SELECT
								pepid
							 FROM
							 	pdeescola.pdeepf
							 WHERE
							 	pdeid = ".$pdeid." AND
							 	epfid = ".$epfid);
		
		if($existe == NULL) {
			$sql = "INSERT INTO pdeescola.pdeepf(pdeid,epfid) VALUES(".$pdeid.",".$epfid.")";
			$db->executar($sql);
		}
		
		$existe_preenchimento = $db->pegaUm("SELECT 
												pprid
											 FROM 
											 	pdeescola.pdepreenchimento
											 WHERE
											 	pdeid = '$pdeid'
											 AND
											 	ppritem = 'p1'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento ,ppritem, pdeanoreferencia ) VALUES ( '$pdeid', 'i1', 'p1','{$_SESSION['exercicio_atual']}')";
			$db->executar( $sql_p );
			
		}
		$db->commit();  
	}
}

/*----------------- Configuraï¿½ï¿½es de Pï¿½gina ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

$pdeid 	= $_SESSION["pdeid"];
$entid  = $_SESSION["entid"];
if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
}

/***********************************************************
 * Componente que irï¿½ montar os butï¿½es do formulï¿½rio
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serï¿½o carregados pelo 
 * 										   __construct "Padrï¿½es ï¿½ classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   nï¿½o precisa dar include do arquivo na pï¿½gina.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos mï¿½todos 
 * 																		1ï¿½ parametro
 * 																		passa a funï¿½ï¿½o que serï¿½ aplicada nos botï¿½es 
 * 																		"anterior, prï¿½ximo, salvar anterior, salvar prï¿½ximo e salvar".
 * 																		2ï¿½ parametro
 * 																		serï¿½ um array de "ï¿½ndice" igual ao parametro da funcï¿½o 
 * 																		passada no 1ï¿½ parametro e o valor serï¿½ o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulï¿½rio "dentro da tabela ainda", este monta os butï¿½es.
 * 											  O parametro passado refere-se a funï¿½ï¿½o javascript que validarï¿½ o formulï¿½rio,
 * 											  devendo esta submeter o formulï¿½rio. 
 * 
 ***********************************************************/

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva()');

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', '');
echo cabecalhoPDE(); 

/*----------------- Recupera dados ---------------------*/
$dados = $db->pegaLinha("SELECT 	pdeuf,
								pdemunicipio, 
								pdenome,
								pdenomediretor,
								pdebairro,
								pdelogradouro,
								pdecomplemento,
								pdetelefone,
								pdeemail,
								tloid
						FROM pdeescola.pdeescola
						WHERE pdeid = '".$pdeid."'");
extract($dados);
if($dados['tloid'] == 1){
	$check1 = 'checked="checked"';
}else if($dados['tloid'] == 2){
	$check2 = 'checked="checked"';
}else if($dados['tloid'] == 3){
	$check3 = 'checked="checked"';
}

$sql = "SELECT entnome as nome
		FROM entidade.entidade as e
		INNER JOIN entidade.funcaoentidade as fe ON fe.entid = e.entid
		WHERE funid = 3 and
		  	  tpcid IN (1,3) AND
	    	  e.entid IN ('{$entid}')";
$NomeEscola = $db->pegaum($sql);
$pdenome = $NomeEscola;

?>
<?=subCabecalho('1-6 - Dados da Escola.'); ?>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<form method="post" id="formulario" name="formulario">
<input type="hidden" id="submetido" name="submetido" value="0">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
  <tr>
    <td class="SubTituloDireita" >Nome da escola:</td>
    <td colspan="3"><?=campo_texto('pdenome', 'S', 'N' , '', 50, 100, '', '', 'left', '', 0, 'id="pdenome"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita">Nome do diretor:</td>
    <td colspan="3"><?=campo_texto('pdenomediretor', 'S', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="pdenomediretor";"'); ?></td>
  </tr>
   <tr>
    <td class="SubTituloDireita" >Telefone:</td>
    <td colspan="3"><?=campo_texto('pdetelefone', 'N', 'S' , '', 50, 10, '##########', '', 'left', '', 0, 'id="pdetelefone"'); ?></td>
   </tr>
   <tr>
    <td class="SubTituloDireita" >E-mail:</td>
    <td  colspan="3" ><?=campo_texto('pdeemail', '', 'S' , '', 50, 100, '', '', 'left', '', 0, 'id="pdeemail"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Endereço da escola:</td>
    <td class="SubTituloDireita" colspan="3"></td>
  </tr>
   <tr>
    <td class="SubTituloDireita">Estado: </td>
    <td  colspan="3"><?=campo_texto('pdeuf', 'S', 'S' , '', 50, 2, '', '', 'left', '', 0, 'id="pdeuf"'); ?></td>
   </tr>
   <tr>
    <td class="SubTituloDireita" >Município:</td>
    <td  colspan="3"><?=campo_texto('pdemunicipio', 'S', 'S' , '', 50, 80, '', '', 'left', '', 0, 'id="pdemunicipio"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Bairro:</td>
    <td colspan="3"><?=campo_texto('pdebairro', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdebairro"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Logradouro:</td>
    <td colspan="3"><?=campo_texto('pdelogradouro', 'S', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdelogradouro"'); ?></td>
  </tr>
  <tr>
    <td class="SubTituloDireita" >Complemento:</td>
    <td colspan="3"><?=campo_texto('pdecomplemento', '', 'S' , '', 50, 255, '', '', 'left', '', 0, 'id="pdecomplemento"'); ?></td>
  </tr>
  <tr>
     <td class="SubTituloDireita" >Localização:</td>
      <td colspan="3"><?=campo_radio('tloid','S','S' , '', 50, 255, '', '', 'left', '', 0, 'id="tloid"'); ?>
       <input id="area" type="radio" name="area" value="1" <?=$check1;?>/>Área urbana <br>
       <input id="area" type="radio" name="area" value="2" <?=$check2;?>/>Área rural <br>
       <input id="area" type="radio" name="area" value="3" <?=$check3;?>/>Área urbana periférica <br> 
  	 </td>
  </tr>
  <? $cForm->montarbuttons("submeterDados()");?>				
</table>
</form>

<script language="javascript" type="text/javascript">
function retorna() {
	window.location = "?modulo=principal/estrutura_avaliacao&acao=A&entid=<?=$entid?>";
}

function proximoFormulario() {
	window.location = "?modulo=principal/instrumento1/f2_nivel_modalidade&acao=A&pdeid=<?=$pdeid?>&entid=<?=$entid?>";
}

function submeterDados() {
    objRadio = '';
	erro = '';
	if(document.getElementById('pdeuf').value == ''){
		erro = "O Estado deve ser informado. \n";
	}
	if(document.getElementById('pdemunicipio').value == ''){
		erro += "O Município deve ser informado.\n";
	}
	if(document.getElementById('pdenome').value == ''){
		erro += "O Nome da escola deve ser informado.\n";
	}
	if(document.getElementById('pdenomediretor').value == ''){
		erro += "O Nome do Diretor deve ser informado.\n" ;
	}
//	if(document.getElementById('pdebairro').value == ''){
//		erro += "O bairro da escola deve ser informado.\n";
//	}
	if(document.getElementById('pdelogradouro').value == ''){
		erro += "O logradouro da escola deve ser informado.\n";
	}
	
//	if(document.getElementById('pdetelefone').value == ''){
//		erro += "O telefone da escola deve ser informado.\n";
//	}

	objRadio = document.formulario.area;
	valorRadio = pegaRadio( objRadio );
	if( valorRadio == false ) {
		    erro += "A localização da escola deve ser informado.\n";
	}
	
	if(erro != ''){
		alert(erro);
		return false;
	}else{
		document.getElementById('submetido').value = 1;
		document.getElementById('formulario').submit();
		return true;
	}
}

function pegaRadio(objRadio){
	checkEmBranco = false;
	if ( objRadio.value )
	{
		return objRadio.value;
	}
	if ( objRadio.length )
	{
		for ( var i = 0 ; i < objRadio.length ; i++ ){
			if (objRadio[i].checked) {
				
				return objRadio[ i ].value;
				break ;
			}else{
				checkEmBranco = true;
			}
		}
	}
	if(!checkEmBranco){
		return objRadio.value;
	}else{
		return false;
	}
}
</script>