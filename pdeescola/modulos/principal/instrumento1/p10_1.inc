<?php
pde_verificaSessao();
/*----- Recuperando $docid para usar na função pegarEstadoAtual ------*/ 
$docid  = pegarDocid( $_SESSION['entid'] );
$estado = pegarEstadoAtual( $docid );
$estadoPerm = (($estado == 76) || ($estado == 37));	

/*----------------- Configurações de Página ---------------------*/ 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

/*----------------- Verificando Perfil ---------------------*/ 
$perfis = arrayPerfil();
$boSuperUsuario = in_array( PDEESC_PERFIL_SUPER_USUARIO, $perfis );
$boPerfilPerm = ((in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_MUNICIPAL, $perfis)) || (in_array(PDEESC_PERFIL_EQUIPE_ESCOLA_ESTADUAL, $perfis)));


/*----------------- Validação ---------------------*/ 
$pdeid	= $_SESSION["pdeid"];
if (!$pdeid){
		die('<script>
				alert(\'Falta parametros!\nTente novamente.\');
				history.go(-1);
			 </script>');
}

if( $_REQUEST['excluir'] != '' )
{
//	$sql = "DELETE FROM 
//					pdeescola.matriculainicial 
//			WHERE 
//					pdeid in (SELECT
//					pdeid
//			FROM
//					pdeescola.aproveitamentoaluno a
//			LEFT JOIN 
//					pdeescola.seriecicloescolar s ON s.sceid = a.sceid
//			WHERE 
//					pdeid = ".$_SESSION["pdeid"]." 
//			AND     a.sceid = ".$serie[$i]["sceid"].")
//			AND sceid = '{$_REQUEST['excluir']}'";
 	$sql = "DELETE FROM pdeescola.matriculainicial WHERE pdeid = $pdeid AND sceid = '{$_REQUEST['excluir']}'";
 	$sql2 = "DELETE FROM pdeescola.aproveitamentoaluno WHERE pdeid = $pdeid AND sceid = '{$_REQUEST['excluir']}'";
// 	//echo $sql;
// 	die();
  	$db->executar( $sql );
  	$db->executar( $sql2 );
  	
	# Se não tiver registro deletamos da tabela preenchimento
	$sqlExclusao = "SELECT * FROM pdeescola.matriculainicial WHERE pdeid = $pdeid AND sceid in(1,2,3,4,5,10,6,7,8,9)";
  	$arMatriculaInicial = $db->carregar( $sqlExclusao );
  	if(!is_array($arMatriculaInicial)){
  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'p10_1'");
  		if($pdeanoreferencia){
			$sqlExclusao = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'p10_1' AND pdeanoreferencia = '".$pdeanoreferencia."'";
			$db->executar( $sqlExclusao );
  		}
  	}
  	
  	if($_REQUEST['excluir'] == 1 
  	 ||$_REQUEST['excluir'] == 2
  	 ||$_REQUEST['excluir'] == 3
  	 ||$_REQUEST['excluir'] == 4
  	 ||$_REQUEST['excluir'] == 5
  	 ||$_REQUEST['excluir'] == 10){
	  	$sqlExclusao = "SELECT * FROM pdeescola.matriculainicial WHERE pdeid = $pdeid AND sceid in(1,2,3,4,5,10)";
	  	$arMatriculaInicial = $db->carregar( $sqlExclusao );
	  	if(!is_array($arMatriculaInicial)){
	  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'p10_2'");
	  		if($pdeanoreferencia){
				$sqlExclusao = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'p10_2' AND pdeanoreferencia = '".$pdeanoreferencia."'";
				$db->executar( $sqlExclusao );
	  		}
	  	}
  	}
  	
  	
	if($_REQUEST['excluir'] == 6 
  	 ||$_REQUEST['excluir'] == 7
  	 ||$_REQUEST['excluir'] == 8
  	 ||$_REQUEST['excluir'] == 9){
	  	$sqlExclusao = "SELECT * FROM pdeescola.matriculainicial WHERE pdeid = $pdeid AND sceid in(6,7,8,9)";
	  	$arMatriculaInicial = $db->carregar( $sqlExclusao );
	  	if(!is_array($arMatriculaInicial)){
	  		$pdeanoreferencia = $db->pegaUm("SELECT pdeanoreferencia FROM pdeescola.pdepreenchimento WHERE pdeid = '$pdeid' AND ppritem = 'p10_3'");
	  		if($pdeanoreferencia){
				$sqlExclusao = "DELETE FROM pdeescola.pdepreenchimento WHERE pdeid = ".$pdeid." AND ppritem = 'p10_3' AND pdeanoreferencia = '".$pdeanoreferencia."'";
				$db->executar( $sqlExclusao );
	  		}
	  	}
  	}
  	
	$db->commit();
	die();
}



/***********************************************************
 * Componente que irá montar os butões do formulário
 * $cDado = array("instrumento" => 1) ==>> Paramentros que serão carregados pelo 
 * 										   __construct "Padrões à classe".
 * $cForm = new formulario($cDado)    ==>> Instanciando a classe, passando os parametros, 
 * 										   não precisa dar include do arquivo na página.
 * $cForm->direcPag('salvaP13( epaid )', array("epaid" => $epaid)); ==>> Faz o gerenciamento dos métodos 
 * 																		1º parametro
 * 																		passa a função que será aplicada nos botões 
 * 																		"anterior, próximo, salvar anterior, salvar próximo e salvar".
 * 																		2º parametro
 * 																		será um array de "índice" igual ao parametro da funcão 
 * 																		passada no 1º parametro e o valor será o desejado.
 * $cForm->montarbuttons("validaForm()") ==>> Localiza-se no final da tabela do formulário "dentro da tabela ainda", este monta os butões.
 * 											  O parametro passado refere-se a função javascript que validará o formulário,
 * 											  devendo esta submeter o formulário. 
 * 
 ***********************************************************/

$cDado = array("instrumento" => 1);
$cForm = new formulario($cDado);
$cForm->direcPag('salva( pdeid )', array("pdeid" => $pdeid));


/*----------------- ações do formulario ---------------------*/ 
function salva($pdeid){	
	global $db;
	$total = 0;
	//dbg($_POST,1);
	if($_POST["submetido"]) {
		$total = count($_POST['numSerie']);
		if($total != 0){
//			$sqlDel = "DELETE FROM pdeescola.matriculainicial WHERE pdeid = ".$pdeid;
//			$sqlDel1 = "DELETE FROM pdeescola.aproveitamentoaluno WHERE pdeid = ".$pdeid;
//			$db->executar($sqlDel);
//			$db->executar($sqlDel1);	
			for ($i = 0; $i < $total; $i++){
				$sceid = $_POST['numSerie'][$i];
				
				$maianee = ((isset($_POST['maianee'][$i])) && ($_POST['maianee'][$i] !='')) ? $_POST['maianee'][$i] : 'null' ;

				if( ($_POST['turmaMat'][$i] != "") && ($_POST['alunoMat'][$i] != "")){
					$titid 				= TURNO_MATUTINO;
					$quantidadeAluno 	= $_POST['alunoMat'][$i];
					$quantidadeTurma	= $_POST['turmaMat'][$i];
					$values[] = ($pdeid.",".$sceid.",".$titid.",".$quantidadeAluno.",".$quantidadeTurma.",".$maianee);
					 
					
				}
				if(($_POST['turmaVes'][$i] != "") && ($_POST['alunoVes'][$i] != "") ){
					$titid 				= TURNO_VESPERTINO;
					$quantidadeAluno 	= $_POST['alunoVes'][$i];
					$quantidadeTurma	= $_POST['turmaVes'][$i];
					$values[] = ($pdeid.",".$sceid.",".$titid.",".$quantidadeAluno.",".$quantidadeTurma.",".$maianee);
				}
				if(($_POST['turmaNot'][$i] != "") && ($_POST['alunoNot'][$i] != "") ){
					$titid 				= TURNO_NOTURNO;
					$quantidadeAluno 	= $_POST['alunoNot'][$i];
					$quantidadeTurma	= $_POST['turmaNot'][$i];
					$values[] = ($pdeid.",".$sceid.",".$titid.",".$quantidadeAluno.",".$quantidadeTurma.",".$maianee);
				}
				if( ($_POST['turmaInt'][$i] != "") && ($_POST['alunoInt'][$i] != "")){
					$titid 				= TURNO_INTEGRAL;
					$quantidadeAluno 	= $_POST['alunoInt'][$i];
					$quantidadeTurma	= $_POST['turmaInt'][$i];
					$values[] = ($pdeid.",".$sceid.",".$titid.",".$quantidadeAluno.",".$quantidadeTurma.",".$maianee);
				}
			}
		}else
		{
			echo("<script>alert('Falta parametros!\nTente novamente.')</script>");
			echo("<script>location.href=window.location:</script>");
			
		}
		if (count($values))
		{ 
				for( $k = 0; $k <count( $values ); $k++)
				{	  
						 
		 				$val = explode(",",$values[$k]);
//		 				echo'<pre>';
// 						print_r( $val );
 						
	 					$sceid 			 = $val[1];
						$titid 			 = $val[2];
						$quantidadeAluno = $val[3];
						$quantidadeTurma = $val[4];
		 				$maianee 		 = $val[5];
		 				 
		 				$sqlmai = "SELECT maiid from pdeescola.matriculainicial 
						where pdeid = $pdeid AND titid = $titid AND sceid = $sceid"; 
						$temmai = $db->pegaUm( $sqlmai );
//						echo $temmai; 
						if($temmai != '')
						{
							$sqlUp = "UPDATE pdeescola.matriculainicial
									  SET
									  	 maiqtdaluno = ".str_replace('\'', '', substr($quantidadeAluno,0,4)).",
									  	 maiqtdTurma = ".str_replace('\'', '', substr($quantidadeTurma,0,3)).",						  	  
									  	 maianee	 = ".str_replace('\'', '', ($maianee = $maianee != 'null' ? substr($maianee,0,3) : $maianee)).",
									  	 maianoreferencia = date_part('year', current_date)
									  WHERE
									  	 maiid = $temmai							
									  ";
//							echo $sqlUp;
//							echo'<br>';
							$up = $db->executar( $sqlUp);
							$db->commit();
						} 
						else
						{
						$sqlins ="INSERT INTO pdeescola.matriculainicial( pdeid,
																			sceid,
																			titid,
																			maiqtdaluno,
																			maiqtdturma,
																			maianee,
																			maianoreferencia)						
							VALUES ( $pdeid, 
							$sceid, $titid, ".str_replace('\'', '', substr($quantidadeAluno,0,4)).", ".str_replace('\'', '', substr($quantidadeTurma,0,3)).", ".str_replace('\'', '', ($maianee = $maianee != 'null' ? substr($maianee,0,3) : $maianee))." ,date_part('year', current_date) 								
							)";

						$db->executar( $sqlins ); 
						}
					} 
			} 
 		  
		$epfid = $db->pegaUm("SELECT
									epfid
								  FROM
								  	pdeescola.estruturaperfilfuncionamento
								  WHERE
								  	trim(epfnomearquivo) = 'p10_1' AND
								  	epfano = ".ANO_EXERCICIO_PDE_ESCOLA);
		
		$existe = $db->pegaUm("SELECT
								pepid
							 FROM
							 	pdeescola.pdeepf
							 WHERE
							 	pdeid = ".$pdeid." AND
							 	epfid = ".$epfid);
		
		if($existe == NULL) 
		{
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
											 	ppritem = 'p10_1' ");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid','i1', 'p10_1', '{$_REQUEST['anoreferencia']}')";
			$db->executar( $sql_p );
	 
		}
			$db->commit();  
	}
}
 
/*----------------- Cabeçalho ---------------------*/ 
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
monta_titulo('Instrumento 1', '');
echo cabecalhoPDE();

/*----------------- Recupera dados e monta formulario ---------------------*/ 
$selectSerie = $db->monta_combo('sceid',
                                "SELECT  sceid AS codigo, 
									scedescricao AS descricao
								FROM pdeescola.seriecicloescolar
								WHERE scetipo = 'S' and tmeid in( 2, 3) 
								ORDER BY scedescricao",
                                'S', 'Escolha...', '', '', '', '', 'N', 'sceid', true);

$dados = $db->carregar("
							SELECT  m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(2, 3) order by m.sceid");

//echo'<pre>';
//print_r( $dados );
//echo'</pre>';
//die();
?>
<?=subCabecalho('10.1 - Matrícula inicial (ano anterior) - Ensino Fundamental'); ?>	
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    <script src="../includes/prototype.js"></script>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" id="submetido" name="submetido" value="0">
<input type="hidden" id="totalform" name="totalform" value="0">
<table class="tabela" bgcolor="#f5f5f5" align="center">

<tr>
  	<td colspan="3">
  		
  		 <?php 
	  		 $anoex   = ANO_EXERCICIO_PDE_ESCOLA;
	  		 $anoex2  = $anoex +1;
    		 $anoex3  = $anoex2 +1;
//  		 $anoex4  = $anoex3 -1;
	  		 $arrUrl  = $_SERVER['argv'][0];
		  	 $arrUrl  = explode("&", $arrUrl);
			 $arrItem = explode("/", $arrUrl[0]);	 
			 $indice  = (count($arrItem) - 1);
		  	 $ppritem = $arrItem[$indice];
	  		 
	  		 $sqlAnoReferencia = "SELECT pdeanoreferencia 
	  		 					  FROM pdeescola.pdepreenchimento 
	  		 					  WHERE pdeid = '{$_SESSION['pdeid']}'
	  		 					  AND ppritem = '$ppritem'";
	  		 
			 $anoreferencia = $db->pegaUm( $sqlAnoReferencia );  		 
  		 
  		 ?>
  		 Ano de Referencia: 
  		 <select name="anoreferencia" id="anoreferencia" class='CampoEstilo'>
  		 	<!-- 
         	<option value=''>Selecione...</option>
         	-->
         	<option value="<?=$anoex;?>"  <? if($anoreferencia ==  $anoex) echo("selected = selected"); ?>><? echo $anoex;?></option>
    		<option value="<?=$anoex2;?>" <? if($anoreferencia == $anoex2) echo("selected = selected"); ?>><? echo $anoex2;?></option>
         	<!--   
         	<option value="<?=$anoex3;?>" <? if($anoreferencia == $anoex3) echo("selected = selected"); ?>><? echo $anoex3;?></option>
         	<option value="<?=$anoex4;?>" <? if($anoreferencia == $anoex4) echo("selected = selected"); ?>><? echo $anoex4;?></option>
			-->
		 </select>  
    </td>
  </tr>
  <tr>
    <td class="SubtituloEsquerda">Série / Ano:</td>
    <td class="SubtituloEsquerda"><?=$selectSerie; ?></td>
    <td class="SubtituloEsquerda">
    	<input type='button' class="botao" name='confirma' value='confirma' onclick="inseriritens(0)" title="Confirmar dados" />
    </td>
  </tr>
</table>
<table class="tabela listagem" id="itensDeMatricula" border="0" align="center" cellpadding="0" cellspacing="4" width="100%" style="text-align: center">
  <tr>
  	<td>Ações</td>
    <td>Série / Ano /  Ciclo  </td>
    <td colspan="2">Matutino</td>
    <td colspan="2">Vespertino</td>
    <td colspan="2">Noturno</td>
    <td colspan="2">Integral</td>
    <td colspan="2">Total</td>
  </tr>
  <tr>
    <td>&nbsp;</td>
    <td>&nbsp;</td>
    <td>Turmas</td>
    <td>Alunos</td>
    <td>Turmas</td>
    <td>Alunos</td>
    <td>Turmas</td>
    <td>Alunos</td>
    <td>Turmas</td>
    <td>Alunos</td>
    <td>Turmas</td>
    <td>Alunos</td>
    <td>ANEE</td>
  </tr>
</table>
<table class="tabela listagem"  id="itensDeMatricula" border="1" align="center" cellpadding="0" cellspacing="4" width="100%" style="text-align: center">
  <tr>

     <td class="SubTituloEsquerda" width="15%" >Total: </td>
    <td width="8%" id="totalRodapeTur1" name="totalRodapeTur1" align="left"></td>
    <td width="8%" id="totalRodapeAlu1" name="totalRodapeAlu1" align="left"></td>
    <td width="8%" id="totalRodapeTur2" name="totalRodapeTur2" align="left"></td>
    <td width="8%" id="totalRodapeAlu2" name="totalRodapeAlu2" align="left"></td>
    <td width="7%" id="totalRodapeTur3" name="totalRodapeTur3" align="left"></td>
    <td width="8%" id="totalRodapeAlu3" name="totalRodapeAlu3" align="left"></td>
    <td width="8%" id="totalRodapeTur4" name="totalRodapeTur4" align="left"></td>
    <td width="8%" id="totalRodapeAlu4" name="totalRodapeAlu4" align="left"></td>
    <td width="8%" id="totalRodapeTotalTur" name="totalRodapeTotalTur" style="background-color: #f0f0f0" align="left"></td>
    <td width="8%" id="totalRodapeTotalAlu" name="totalRodapeTotalAlu" style="background-color: #f0f0f0" align="left"></td>
    <td width="8%" id="totalRodapeTotalAnee" name="totalRodapeTotalAnee" style="background-color: #f0f0f0" align="left"></td>
    <td class="SubTituloEsquerda" width="8%" align="left"></td>
    <td class="SubTituloEsquerda" width="8%" align="left"></td>
    <td class="SubTituloEsquerda" width="8%" align="left"></td>
  </tr>
</table>
<table class="tabela" bgcolor="#f5f5f5" align="center">
 <? $cForm->montarbuttons("submeterDados()");?>				
</table>
</form>

<script language="javascript" type="text/javascript">
var arraySerie = new Array();
var alunoMat1;
var alunoVes2;
var alunoNot3;
var alunoInt4;
var turmaMat1;
var turmaVes2;
var turmaNot3;
var turmaInt4;
var maianee;

function inseriritens(sceid, descricao){
	if(sceid != 0){
		var serie = Number(sceid); 
		var textoSerie = descricao;
	}else{
		var serie = Number(document.getElementById("sceid").value);
		textoSerie = document.getElementById("sceid");
		var textoSerie = textoSerie.options[textoSerie.selectedIndex].text;
	}
	// validar dados
	if(serie == 0){
		alert("Escolha alguma serie escolar.");
		return false;
	}
	var totalArray = arraySerie.length;
	for (cont=0;cont<=totalArray;cont++){
		if(arraySerie[cont] == serie){
			alert("Já existe um Série cadastrada.");
			return false;
		}else{
			continue;
		}
	}
	// Gera componentes do formulario.
	arraySerie.push(serie);
	var btnExcluirItem     		= document.createElement('img');
        btnExcluirItem.src     	= '/imagens/excluir.gif';
        btnExcluirItem.onclick 	= function(){
        	removerItem(this.parentNode.parentNode, sceid);
        }
    
    input_serie  = document.createElement('input');
    input_serie.type 	=  'hidden';
    input_serie.name 	=  'numSerie[]';
    input_serie.id 		=  'numSerie[]';
    input_serie.value 	=  serie;
    //input_serie.setAttribute('type', 'hidden');
    //input_serie.setAttribute('name', 'numSerie[]');
    //input_serie.setAttribute('id', 'numSerie[]');
    //input_serie.setAttribute('value', serie);    

	input_turmaM    = document.createElement('input');
    input_turmaM.setAttribute('type', 'text');
    input_turmaM.setAttribute('class', 'normal');
    input_turmaM.setAttribute('name', 'turmaMat[]');
    input_turmaM.setAttribute('id', 'turmaMat[]');
    input_turmaM.setAttribute('maxlength', '3');
    input_turmaM.setAttribute('size', '10');
    //input_turmaM.setAttribute('onkeyup', 'function valorTurmaLinha()');
    input_turmaM.onkeyup =  function () {valorTurmaLinha();};
    //input_turmaM.setAttribute('onkeypress', 'return somenteNumeros(event);');
    //input_turmaM.setAttribute('onchange', 'return onlyNumbers(this.id);'); 
    //testes
      if( document.all){
         input_turmaM.onkeypress = function () { return somenteNumeros(event); };
         input_turmaM.onblur 	 = function () { onlyNumbers(this.id); };
         input_turmaM.onchange   = function () { onlyNumbers(this.id); };
         input_turmaM.onkeyup    = function () { verifica(this); };
      }
      else{
         input_turmaM.setAttribute("onblur","return onlyNumbers(this.id);");
         input_turmaM.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_turmaM.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
	
    input_turmaV    = document.createElement('input');
    input_turmaV.setAttribute('type', 'text');
    input_turmaV.setAttribute('class', 'normal');
    input_turmaV.setAttribute('name', 'turmaVes[]');
    input_turmaV.setAttribute('id', 'turmaVes[]');
    input_turmaV.setAttribute('maxlength', '3');
    input_turmaV.setAttribute('size', '10');
   // input_turmaV.setAttribute('onkeyup', 'valorTurmaLinha()');
    input_turmaV.onkeyup =  function () {valorTurmaLinha();};
         if( document.all){
         input_turmaV.onkeypress = function () { return somenteNumeros(event); };
         input_turmaV.onblur 	 = function () { onlyNumbers(this.id); };
         input_turmaV.onchange   = function () { onlyNumbers(this.id); };
         input_turmaV.onkeyup    = function () { verifica(this); };
      }
      else{
         input_turmaV.setAttribute("onblur","return onlyNumbers(this.id);");
         input_turmaV.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_turmaV.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    input_turmaN    = document.createElement('input');
    input_turmaN.setAttribute('type', 'text');
    input_turmaN.setAttribute('class', 'normal');
    input_turmaN.setAttribute('name', 'turmaNot[]');
    input_turmaN.setAttribute('id', 'turmaNot[]');
    input_turmaN.setAttribute('maxlength', '3');
    input_turmaN.setAttribute('size', '10');
    //input_turmaN.setAttribute('onkeyup', 'valorTurmaLinha()');
    input_turmaN.onkeyup =  function () {valorTurmaLinha();};
         if( document.all){
         input_turmaN.onkeypress = function () { return somenteNumeros(event); };
         input_turmaN.onblur 	 = function () { onlyNumbers(this.id); };
         input_turmaN.onchange   = function () { onlyNumbers(this.id); };
         input_turmaN.onkeyup    = function () { verifica(this); };
      }
      else{
         input_turmaN.setAttribute("onblur","return onlyNumbers(this.id);");
         input_turmaN.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_turmaN.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    input_turmaI    = document.createElement('input');
    input_turmaI.setAttribute('type', 'text');
    input_turmaI.setAttribute('class', 'normal');
    input_turmaI.setAttribute('name', 'turmaInt[]');
    input_turmaI.setAttribute('id', 'turmaInt[]');
    input_turmaI.setAttribute('maxlength', '3');
    input_turmaI.setAttribute('size', '10');
    //input_turmaI.setAttribute('onkeyup', 'valorTurmaLinha()');
    input_turmaI.onkeyup =  function () {valorTurmaLinha();};
          if( document.all){
         input_turmaI.onkeypress = function () { return somenteNumeros(event); };
         input_turmaI.onblur 	 = function () { onlyNumbers(this.id); };
         input_turmaI.onchange   = function () { onlyNumbers(this.id); };
         input_turmaI.onkeyup    = function () { verifica(this); };
      }
      else{
         input_turmaI.setAttribute("onblur","return onlyNumbers(this.id);");
         input_turmaI.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_turmaI.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    input_resultTurma    = document.createElement('input');
    input_resultTurma.setAttribute('type', 'text');
    input_resultTurma.setAttribute('class', 'normal');
    input_resultTurma.setAttribute('name', 'valorTurma[]');
    input_resultTurma.setAttribute('id', 'valorTurma[]');
    input_resultTurma.setAttribute('maxlength', '3');
    input_resultTurma.setAttribute('size', '10');
    input_resultTurma.setAttribute('readOnly', 'readonly');
    input_resultTurma.style.backgroundColor = '#F0F0F0'; 
          if( document.all){
         input_resultTurma.onkeypress = function () { return somenteNumeros(event); };
         input_resultTurma.onblur 	  = function () { onlyNumbers(this.id); };
         input_resultTurma.onchange   = function () { onlyNumbers(this.id); };
         input_resultTurma.onkeyup    = function () { verifica(this); };
      }
      else{
         input_resultTurma.setAttribute("onblur","return onlyNumbers(this.id);");
         input_resultTurma.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_resultTurma.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    input_alunoM    = document.createElement('input');
    input_alunoM.setAttribute('type', 'text');
    input_alunoM.setAttribute('class', 'normal');
    input_alunoM.setAttribute('name', 'alunoMat[]');
    input_alunoM.setAttribute('id', 'alunoMat[]');
    input_alunoM.setAttribute('maxlength', '4');
    input_alunoM.setAttribute('size', '10');
    //input_alunoM.setAttribute('onkeyup', 'valorAlunoLinha()');
    input_alunoM.onkeyup =  function () {valorAlunoLinha();};
         if( document.all){
         input_alunoM.onkeypress = function () { return somenteNumeros(event); };
         input_alunoM.onblur 	 = function () { onlyNumbers(this.id); };
         input_alunoM.onchange   = function () { onlyNumbers(this.id); };
         input_alunoM.onkeyup    = function () { verifica2(this); };
      }
      else{
         input_alunoM.setAttribute("onblur","return onlyNumbers(this.id);");
         input_alunoM.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_alunoM.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    input_alunoV    = document.createElement('input');
    input_alunoV.setAttribute('t.lengthype', 'text');
    input_alunoV.setAttribute('class', 'normal');
    input_alunoV.setAttribute('name', 'alunoVes[]');
    input_alunoV.setAttribute('id', 'alunoVes[]');
    input_alunoV.setAttribute('maxlength', '4');
    input_alunoV.setAttribute('size', '10');
    //input_alunoV.setAttribute('onkeyup', 'valorAlunoLinha()');
    input_alunoV.onkeyup =  function () {valorAlunoLinha();};
         if( document.all){
         input_alunoV.onkeypress = function () { return somenteNumeros(event); };
         input_alunoV.onblur 	 = function () { onlyNumbers(this.id); };
         input_alunoV.onchange   = function () { onlyNumbers(this.id); };
         input_alunoV.onkeyup    = function () { verifica2(this); };
      }
      else{
         input_alunoV.setAttribute("onblur","return onlyNumbers(this.id);");
         input_alunoV.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_alunoV.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    input_alunoN    = document.createElement('input');
    input_alunoN.setAttribute('type', 'text');
    input_alunoN.setAttribute('class', 'normal');
    input_alunoN.setAttribute('name', 'alunoNot[]');
    input_alunoN.setAttribute('id', 'alunoNot[]');
    input_alunoN.setAttribute('maxlength', '4');
    input_alunoN.setAttribute('size', '10');
    //input_alunoN.setAttribute('onkeyup', 'valorAlunoLinha()');
    input_alunoN.onkeyup =  function () {valorAlunoLinha();};
          if( document.all){
         input_alunoN.onkeypress = function () { return somenteNumeros(event); };
         input_alunoN.onblur 	 = function () { onlyNumbers(this.id); };
         input_alunoN.onchange   = function () { onlyNumbers(this.id); };
         input_alunoN.onkeyup    = function () { verifica2(this); };
      }
      else{
         input_alunoN.setAttribute("onblur","return onlyNumbers(this.id);");
         input_alunoN.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_alunoN.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    input_alunoI    = document.createElement('input');
    input_alunoI.setAttribute('type', 'text');
    input_alunoI.setAttribute('class', 'normal');
    input_alunoI.setAttribute('name', 'alunoInt[]');
    input_alunoI.setAttribute('id', 'alunoInt[]');
    input_alunoI.setAttribute('maxlength', '4');
    input_alunoI.setAttribute('size', '10');
    //input_alunoI.setAttribute('onkeyup', 'valorAlunoLinha()');
    input_alunoI.onkeyup =  function () {valorAlunoLinha();};
       if( document.all){
         input_alunoI.onkeypress = function () { return somenteNumeros(event); };
         input_alunoI.onblur 	 = function () { onlyNumbers(this.id); };
         input_alunoI.onchange   = function () { onlyNumbers(this.id); };
         input_alunoI.onkeyup    = function () { verifica2(this); };
      }
      else{
         input_alunoI.setAttribute("onblur","return onlyNumbers(this.id);");
         input_alunoI.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_alunoI.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }    
    input_resultAluno = document.createElement('input');
    input_resultAluno.setAttribute('type', 'text');
    input_resultAluno.setAttribute('class', 'normal');
    input_resultAluno.setAttribute('name', 'valorAluno[]');
    input_resultAluno.setAttribute('id', 'valorAluno[]');
    input_resultAluno.setAttribute('size', '10');
    input_resultAluno.setAttribute('readOnly', 'readonly');
    input_resultAluno.style.backgroundColor = '#F0F0F0'; 
    
    input_anee = document.createElement('input');
    input_anee.setAttribute('type', 'text');
    input_anee.setAttribute('class', 'normal');
    input_anee.setAttribute('name', 'maianee[]');
    input_anee.setAttribute('id', 'maianee[]');
    input_anee.setAttribute('maxlength', '3');
    input_anee.setAttribute('size', '10');  
          if( document.all){
         input_anee.onkeypress = function () { return somenteNumeros(event); };
         input_anee.onblur 	   = function () { onlyNumbers(this.id); };
         input_anee.onchange   = function () { onlyNumbers(this.id); };
         input_anee.onkeyup    = function () { verifica(this); };
      }
      else{
         input_anee.setAttribute("onblur","return onlyNumbers(this.id);");
         input_anee.setAttribute('onkeypress', 'return somenteNumeros(event);');
    	 input_anee.setAttribute('onchange', 'return onlyNumbers(this.id);');
      }
    
    var tabelaMatriculas = document.getElementById('itensDeMatricula');
	var novaLinha = tabelaMatriculas.insertRow(tabelaMatriculas.rows.length);
	novaLinha.id = tabelaMatriculas.rows.length;
	novaLinha.style.backgroundColor  = '#f0f0f0';
	for (i=0;i<=12;i++){
		novaLinha.insertCell(i);
	}
	<?
	if( $estadoPerm || $boSuperUsuario)
	{ 
		if( $boSuperUsuario || $boPerfilPerm )
		{?>
		novaLinha.cells[0].appendChild(btnExcluirItem);
	<?
		}
	}
	?>
	novaLinha.cells[12].appendChild(input_serie);
	novaLinha.cells[1].innerHTML= textoSerie;
	novaLinha.cells[2].appendChild(input_turmaM);
	novaLinha.cells[3].appendChild(input_alunoM);
	novaLinha.cells[4].appendChild(input_turmaV);
	novaLinha.cells[5].appendChild(input_alunoV);
	novaLinha.cells[6].appendChild(input_turmaN);
	novaLinha.cells[7].appendChild(input_alunoN);
	novaLinha.cells[8].appendChild(input_turmaI);
	novaLinha.cells[9].appendChild(input_alunoI);
	novaLinha.cells[10].appendChild(input_resultTurma);
	novaLinha.cells[11].appendChild(input_resultAluno);
	novaLinha.cells[12].appendChild(input_anee);
}

// Joga a soma dos valores totais das turmas na linha corespondente.
function valorTurmaLinha(){
	var tamanho = document.getElementsByName("turmaMat[]").length;

	for (i=0;i<=tamanho-1;i++){
		var turma1 = Number (document.getElementsByName("turmaMat[]")[i].value);
		var turma2 = Number (document.getElementsByName("turmaVes[]")[i].value);
		var turma3 = Number (document.getElementsByName("turmaNot[]")[i].value);
		var turma4 = Number (document.getElementsByName("turmaInt[]")[i].value);
		var totalTurma = turma1 + turma2 + turma3 + turma4;
		var totalTur = totalTurma;
		document.getElementsByName('valorTurma[]')[i].value = totalTurma;
		document.getElementsByName('valorTurma[]')[i].value = totalTur;
	}
}
// Joga a soma dos valores totais dos alunos na linha corespondente.
function valorAlunoLinha(){
	
	var tamanho = document.getElementsByName("alunoMat[]").length;
	for (i=0;i<=tamanho-1;i++){
		var aluno1 = Number (document.getElementsByName("alunoMat[]")[i].value);
		var aluno2 = Number (document.getElementsByName("alunoVes[]")[i].value);
		var aluno3 = Number (document.getElementsByName("alunoNot[]")[i].value);
		var aluno4 = Number (document.getElementsByName("alunoInt[]")[i].value);
		var totalAluno = aluno1 + aluno2 + aluno3 + aluno4;
		var totalAlu = totalAluno;
		document.getElementsByName('valorAluno[]')[i].value = totalAluno;
		document.getElementsByName('valorAluno[]')[i].value = totalAlu;
	}
}

// Joga a soma dos valores totais dos alunos de cada coluna.
var aluno1 = 0;
var aluno2 = 0;
var aluno3 = 0;
var aluno4 = 0;
var totalAlu = 0;

function valorAlunoColuna(){
	var tamanho = document.getElementsByName("alunoMat[]").length;
	for (i=0;i<=tamanho-1;i++){
		aluno1 = Number (document.getElementsByName("alunoMat[]")[i].value) + aluno1 ;
	    aluno2 = Number (document.getElementsByName("alunoVes[]")[i].value) + aluno2;
		aluno3 = Number (document.getElementsByName("alunoNot[]")[i].value) + aluno3;
		aluno4 = Number (document.getElementsByName("alunoInt[]")[i].value) + aluno4;
		totalAlu = Number (document.getElementsByName("valorAluno[]")[i].value) + totalAlu;
	
		document.getElementById('totalRodapeAlu1').innerHTML = aluno1;
		document.getElementById('totalRodapeAlu2').innerHTML = aluno2;
		document.getElementById('totalRodapeAlu3').innerHTML = aluno3;
		document.getElementById('totalRodapeAlu4').innerHTML = aluno4;
		document.getElementById('totalRodapeTotalAlu').innerHTML = totalAlu;
	}
}

// Joga a soma dos valores totais das turmas de cada coluna.
var turma1 = 0;
var turma2 = 0;
var turma3 = 0;
var turma4 = 0;
var totalTur = 0;

function valorTurmaColuna(){
	var tamanho = document.getElementsByName("alunoMat[]").length;
	for (i=0;i<=tamanho-1;i++){
		 turma1 = Number (document.getElementsByName("turmaMat[]")[i].value) + turma1;
		 turma2 = Number (document.getElementsByName("turmaVes[]")[i].value) + turma2;
		 turma3 = Number (document.getElementsByName("turmaNot[]")[i].value) + turma3;
		 turma4 = Number (document.getElementsByName("turmaInt[]")[i].value) + turma4;
		 totalTur = Number (document.getElementsByName("valorTurma[]")[i].value) + totalTur;
	
		document.getElementById('totalRodapeTur1').innerHTML = turma1;
		document.getElementById('totalRodapeTur2').innerHTML = turma2;
		document.getElementById('totalRodapeTur3').innerHTML = turma3;
		document.getElementById('totalRodapeTur4').innerHTML = turma4;
		document.getElementById('totalRodapeTotalTur').innerHTML = totalTur;

	}
}
// Joga a soma dos valores totais das anee de cada coluna.
var anee = 0;

function valorAnee(){
	var tamanho = document.getElementsByName("alunoMat[]").length;
	for (i=0;i<=tamanho-1;i++){
		 anee = Number (document.getElementsByName("maianee[]")[i].value) + anee;
		 	
		document.getElementById('totalRodapeTotalAnee').innerHTML = anee;
		
	}
}

// Remove linha
function removerItem(linha, sceid ){
	 
	var linhaID = linha.getAttribute("id");
        linha.style.backgroundColor = 'rgb(255,255,210)';
        linha.style.backgroundColor = '#DAE0D2';

        if (!confirm('Atenção! Os dados serão apagados permanentemente!\nDeseja realmente excluir o item selecionado?')) {
            linha.style.backgroundColor = '#f0f0f0';
        } else {
        	 var exc = new Ajax.Request(window.location.href,
                                               {
                                                   method: 'post',
                                                   parameters: 'excluir=' + sceid,
                                                   onComplete: function(res)
                                                   {
                                                   	    linha.parentNode.parentNode.deleteRow(linha.rowIndex);
                                                   }
                                               });
        
	 			 		
        }
}

function submeterDados(){
	existeserie = document.getElementsByName('numSerie[]').length;
	if(existeserie == 0){
		alert("Obrigatório inserir pelo menos um campo escolar.");
		return false;
	}
	var tamanho = document.getElementsByName("turmaMat[]").length;
	if(tamanho != 0){
		for (i=0;i<=tamanho-1;i++){
			var t1 = Number (document.getElementsByName("turmaMat[]")[i].value);
			var t2 = Number (document.getElementsByName("turmaVes[]")[i].value);
			var t3 = Number (document.getElementsByName("turmaNot[]")[i].value);
			var t4 = Number (document.getElementsByName("turmaInt[]")[i].value);
			var total = t1 + t2 + t3 + t4;
		}
		if(total == 0){
			alert("Pelo menos algum turno deve ser preenchido.");
			return false;
		}
	}	
	document.getElementById('submetido').value = 1;
	document.getElementById('formulario').submit();
	return true;
}

<? 
$linha = 0;
if(is_array($dados)){

	foreach($dados as $dados ){
		if($linha != $dados['sceid']){
?>
	//alert( 'tst' );
		inseriritens(<?=$dados['sceid']; ?>, '<?=trim($dados['scedescricao']);?>');
<?
		}
		if($linha == 0){
			$linha = $dados['sceid'];
		}
		if($dados['titid'] == TURNO_MATUTINO ){
		?>
			alunoMat1 = <?=$dados['maiqtdaluno'];?>;
			turmaMat1 = <?=$dados['maiqtdturma'];?>;
		<? 
		}
		if($dados['titid'] == TURNO_VESPERTINO ){
		?>
			alunoVes2 = <?=$dados['maiqtdaluno'];?>;
			turmaVes2 = <?=$dados['maiqtdturma'];?>;
		<? 	
		}
		if($dados['titid'] == TURNO_NOTURNO ){
		?>
			alunoNot3 = <?=$dados['maiqtdaluno'];?>;
			turmaNot3 = <?=$dados['maiqtdturma'];?>;
		<? 	
		}
		if($dados['titid'] == TURNO_INTEGRAL ){
		?>
			alunoInt4 = <?=$dados['maiqtdaluno'];?>;
			turmaInt4 = <?=$dados['maiqtdturma'];?>;
		<? 
		}
		if($dados['maianee']){
		?>
			maianee = <?=$dados['maianee'];?>;		
<? 	
		}
?>
		sceid = <?=$dados['sceid'];?>;
		if(turmaMat1){ 
	    	input_turmaM.setAttribute('value', turmaMat1); 
	    	 turmaMat1 = 0;  
	    }
	     if(turmaVes2){
	    	input_turmaV.setAttribute('value', turmaVes2); 
	    	turmaVes2 = 0;
	    }
	    if(turmaInt4){
	    	input_turmaI.setAttribute('value', turmaInt4); 
	    	turmaInt4 = 0;
	    }
	     if(turmaNot3){
	    	input_turmaN.setAttribute('value', turmaNot3); 
	    	turmaNot3 = 0;
	    }
	    if(alunoMat1){
	    	input_alunoM.setAttribute('value', alunoMat1); 
	    	alunoMat1 = 0;
	   	 }
	   	  if(alunoVes2){
		    input_alunoV.setAttribute('value', alunoVes2); 
		    alunoVes2 = 0;
	    }
	    if(alunoNot3){
		    input_alunoN.setAttribute('value', alunoNot3); 
		    alunoNot3 = 0;
	    }
	    if(alunoInt4){
		    input_alunoI.setAttribute('value', alunoInt4); 
		    alunoInt4 = 0;
	    }
	 
	    if(maianee){
		    input_anee.setAttribute('value', maianee); 
		    maianee = 0;
	    }
		valorAlunoLinha();
		valorTurmaLinha();
<?	
		$linha = $dados['sceid'];
	}
}
?>

function verifica(campo) {
	if (campo.value.length >= 3) {
		//alert("Paro!!!");
		campo.value = campo.value.substring(0,3);
	}
}

function verifica2(campo) {
	if (campo.value.length >= 4) {
		//alert("Paro!!!");
		campo.value = campo.value.substring(0,4);
	}
}

function onlyNumbers(id)
{
	//alert('chhegou ie');
	var tamanho = document.getElementsByName(id).length;
	//alert(tamanho);
	for(var i=0;i<tamanho;i++)
	{
		var campo = document.getElementsByName(id)[i];
		if( isNaN( campo.value ) )
		{
			campo.value = '';
		} 
	}
}
	
valorAlunoColuna();
valorTurmaColuna();
valorAnee();

</script>