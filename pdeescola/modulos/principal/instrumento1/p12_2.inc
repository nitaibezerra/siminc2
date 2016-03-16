<?php
pde_verificaSessao();

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

function salva() {
	global $db;
	
	if($_POST["submetido"]) {
		$pdeid = $_POST["pdeid"];
		$serieId = $_POST["serie_id"];
		$apaqtdmatriculainicial = $_POST["apaqtdmatriculainicial"];
		$apaqtdadmitidosaposmarco = $_POST["apaqtdadmitidosaposmarco"];
		$apaqtdafastadosabandono = $_POST["apaqtdafastadosabandono"];
		$apaqtdafastadostransferencia = $_POST["apaqtdafastadostransferencia"];
		$apaqtdmatriculafinal = $_POST["apaqtdmatriculafinal"];
		$apaqtdaprovados = $_POST["apaqtdaprovados"];
		$apaqtdreprovados = $_POST["apaqtdreprovados"];
		
		for($i=0; $i<count($serieId); $i++) {
			$existe = $db->pegaUm("SELECT apaid FROM pdeescola.aproveitamentoaluno
								   WHERE pdeid = ".$pdeid."
								   	 AND sceid = ".$serieId[$i]."
								     AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			
			$apaqtdmatriculainicial[$serieId[$i]] = ($apaqtdmatriculainicial[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdmatriculainicial[$serieId[$i]];
			$apaqtdadmitidosaposmarco[$serieId[$i]] = ($apaqtdadmitidosaposmarco[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdadmitidosaposmarco[$serieId[$i]];
			$apaqtdafastadosabandono[$serieId[$i]] = ($apaqtdafastadosabandono[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdafastadosabandono[$serieId[$i]];
			$apaqtdafastadostransferencia[$serieId[$i]] = ($apaqtdafastadostransferencia[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdafastadostransferencia[$serieId[$i]];
			$apaqtdmatriculafinal[$serieId[$i]] = ($apaqtdmatriculafinal[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdmatriculafinal[$serieId[$i]];
			$apaqtdaprovados[$serieId[$i]] = ($apaqtdaprovados[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdaprovados[$serieId[$i]];
			$apaqtdreprovados[$serieId[$i]] = ($apaqtdreprovados[$serieId[$i]] == NULL) ? 'NULL' : $apaqtdreprovados[$serieId[$i]];
			
			if($existe == "") {
				$db->executar("INSERT INTO pdeescola.aproveitamentoaluno(pdeid,sceid,apaqtdmatriculainicial,
							   apaqtdadmitidosaposmarco,apaqtdafastadosabandono,apaqtdafastadostransferencia,
							   apaqtdmatriculafinal,apaqtdaprovados,apaqtdreprovados,apaanoreferencia,apadataatualizacao) 
							   VALUES(".$pdeid.",".$serieId[$i].",".$apaqtdmatriculainicial[$serieId[$i]].",
							   ".$apaqtdadmitidosaposmarco[$serieId[$i]].",
							   ".$apaqtdafastadosabandono[$serieId[$i]].",
							   ".$apaqtdafastadostransferencia[$serieId[$i]].",
							   ".$apaqtdmatriculafinal[$serieId[$i]].",
							   ".$apaqtdaprovados[$serieId[$i]].",
							   ".$apaqtdreprovados[$serieId[$i]].",
							   ".ANO_EXERCICIO_PDE_ESCOLA.",now())");
			} else {
				$db->executar("UPDATE 
								   pdeescola.aproveitamentoaluno
							   SET 
							   	   apaqtdmatriculainicial = ".$apaqtdmatriculainicial[$serieId[$i]].",
							   	   apaqtdadmitidosaposmarco = ".$apaqtdadmitidosaposmarco[$serieId[$i]].",
							   	   apaqtdafastadosabandono = ".$apaqtdafastadosabandono[$serieId[$i]].",
							   	   apaqtdafastadostransferencia = ".$apaqtdafastadostransferencia[$serieId[$i]].",
							   	   apaqtdmatriculafinal = ".$apaqtdmatriculafinal[$serieId[$i]].",
							   	   apaqtdaprovados = ".$apaqtdaprovados[$serieId[$i]].",
							   	   apaqtdreprovados = ".$apaqtdreprovados[$serieId[$i]]."
							   WHERE 
							   		pdeid = ".$pdeid." AND
								    sceid = ".$serieId[$i]." AND
									apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			}
		}
		
		$epfid = $db->pegaUm("SELECT
								epfid
							  FROM
							  	pdeescola.estruturaperfilfuncionamento
							  WHERE
							  	trim(epfnomearquivo) = 'p12_2' AND
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
											 	ppritem = 'p12_2'");
		if( $existe_preenchimento == NULL )
		{
			$sql_p = "INSERT INTO pdeescola.pdepreenchimento( pdeid, pprinstrumento, ppritem, pdeanoreferencia) VALUES ( '$pdeid', 'i1', 'p12_2', '{$_REQUEST['anoreferencia']}')";
			$db->executar( $sql_p ); 
		}
		$db->commit();  
	}
}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo_modulo = 'Instrumento 1';
monta_titulo($titulo_modulo, '');
echo cabecalhoPDE();

$pdeid = $_SESSION["pdeid"];

if (!$pdeid){
	die('<script>
			alert(\'Falta parametros!\nTente novamente.\');
			history.go(-1);
		 </script>');
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
$cForm->direcPag('salva()');

?>

<script language="javascript" type="text/javascript">
	var series = new Array();
</script>
<br />
<?=subCabecalho('12.2 - Aproveitamento dos alunos da 1ª - 8ª Etapa / Segmento / Totalidade - EJA (ano anterior) '); ?>
<form method="post" id="formulario" name="formulario">
<input type="hidden" name="submetido" value="1">
<input type="hidden" name="pdeid" value="<?=$pdeid?>">
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
<thead>
	<tr>
  	<td colspan="11">
  		
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
		<th>ETAPA</th>
		<th>Matrícula Inicial</th>
		<th>Admitidos após mês de maio</th>
		<th>Afastados por Abandono</th>
		<th>Afastados por Transferências</th>
		<th>Matrícula Final</th>
		<th>Aprovados</th>
		<th>Reprovados</th>
		<th>Taxa de Aprovação</th>
		<th>Taxa de Reprovação</th>
		<th>Taxa de Abandono</th>
	</tr>
</thead>
<tbody>
	<?php
		$serie = $db->carregar("SELECT sceid,scedescricao FROM pdeescola.seriecicloescolar 
								WHERE tmeid = 10 AND scetipo = 'S' ORDER BY sceid");
		
		$totalMatInicial = 0;
		$totalAdmitido = 0;
		$totalAbandono = 0;
		$totalTransferencia = 0;
		$totalMatFinal = 0;
		$totalAprovados = 0;
		$totalReprovados = 0;
		
		for($i=0; $i<count($serie); $i++) {
			echo "<script type=\"text/javascript\"> series.push('".$serie[$i]["sceid"]."'); </script>";
			
			$dados = $db->carregar("SELECT * FROM pdeescola.aproveitamentoaluno
								  	WHERE pdeid = ".$pdeid." 
									  AND sceid = ".$serie[$i]["sceid"]."
									  AND apaanoreferencia = ".ANO_EXERCICIO_PDE_ESCOLA);
			 
			for($j=0; $j<count($dados); $j++) {
				$div = ($dados[$j]["apaqtdmatriculainicial"] + $dados[$j]["apaqtdadmitidosaposmarco"] - $dados[$j]["apaqtdafastadostransferencia"]);
				
				if($div > 0) {
					@$taxa_aprovacao = str_replace('.',',',number_format(($dados[$j]["apaqtdaprovados"] / ($div ) * 100),1))."%";
					@$taxa_reprovacao = str_replace('.',',',number_format(($dados[$j]["apaqtdreprovados"] / ($div ) * 100),1))."%";
					@$taxa_abandono = str_replace('.',',',number_format(($dados[$j]["apaqtdafastadosabandono"] / ($div ) * 100),1))."%";
				}
				else {
					$taxa_aprovacao = "0%";
					$taxa_reprovacao = "0%";
					$taxa_abandono = "0%";
				}
				
				$totalMatInicial += $dados[$j]["apaqtdmatriculainicial"];
				$totalAdmitido += $dados[$j]["apaqtdadmitidosaposmarco"];
				$totalAbandono += $dados[$j]["apaqtdafastadosabandono"];
				$totalTransferencia += $dados[$j]["apaqtdafastadostransferencia"];
				$totalMatFinal += $dados[$j]["apaqtdmatriculafinal"];
				$totalAprovados += $dados[$j]["apaqtdaprovados"];
				$totalReprovados += $dados[$j]["apaqtdreprovados"];
				
				$sqlTotal = "SELECT distinct m.pdeid,
								m.sceid,
								m.titid,
								m.maiqtdaluno,
								m.maiqtdturma,
								m.maianoreferencia,
								m.maianee,
								s.scedescricao
								FROM pdeescola.matriculainicial m
								LEFT JOIN pdeescola.seriecicloescolar s ON s.sceid = m.sceid
								WHERE pdeid = ".$pdeid." and s.tmeid in(10) 
								AND m.sceid = ".$serie[$i]["sceid"]."
								order by m.sceid
								";
			$rsTotal = $db->carregar( $sqlTotal);

			for( $x = 0; $x <count( $rsTotal ); $x++)
			{
				$qtdTotalAluno[$serie[$i]["sceid"]] += $rsTotal[$x]['maiqtdaluno'];
			}

			if( !$qtdTotalAluno[$serie[$i]["sceid"]])
			$value = $dados[$j]["apaqtdmatriculainicial"];
			else
			$value = $qtdTotalAluno[$serie[$i]["sceid"]];
			
			if(!$value){
				$readOnly = 'readonly="true;"';
			}else{
				unset($readOnly);
			}
			
			echo "<tr height=\"30px\" align=\"center\">
					  	<td bgcolor=\"#f0f0f0\">
					  		<b>".$serie[$i]["scedescricao"]."</b>
					  		<input type=\"hidden\" name=\"serie_id[]\" value=\"".$serie[$i]["sceid"]."\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input class=\"normal;\"disabled;\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculainicial[".$serie[$i]["sceid"]."]\" value=\"$value\" onblur=\"alteraTaxa('apaqtdmatriculainicial',".$serie[$i]["sceid"].",'totalMatInicial');\" onkeypress=\"return somenteNumeros(event);\"readonly=\"true;\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" name=\"apaqtdadmitidosaposmarco[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdadmitidosaposmarco"]."\" onblur=\"alteraTaxa('apaqtdadmitidosaposmarco',".$serie[$i]["sceid"].",'totalAdmitido');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadosabandono[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadosabandono"]."\" onblur=\"alteraTaxa('apaqtdafastadosabandono',".$serie[$i]["sceid"].",'totalAbandono');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" name=\"apaqtdafastadostransferencia[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdafastadostransferencia"]."\" onblur=\"alteraTaxa('apaqtdafastadostransferencia',".$serie[$i]["sceid"].",'totalTransferencia');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  <td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" name=\"apaqtdmatriculafinal[".$serie[$i]["sceid"]."]\" value=\"".(($dados[$j]["apaqtdmatriculafinal"] == "") ? 0 : $dados[$j]["apaqtdmatriculafinal"])."\" readonly=\"true\" >
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdaprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdaprovados"]."\"  onblur=\"alteraTaxa('apaqtdaprovados',".$serie[$i]["sceid"].",'totalAprovados');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\">
					  		<input ".$readOnly." class=\"normal\" maxlength=\"3\" type=\"text\" size=\"5\" id=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" name=\"apaqtdreprovados[".$serie[$i]["sceid"]."]\" value=\"".$dados[$j]["apaqtdreprovados"]."\"  onblur=\"alteraTaxa('apaqtdreprovados',".$serie[$i]["sceid"].",'totalReprovados');\" onkeypress=\"return somenteNumeros(event);\">
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_aprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_aprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_reprovacao[".$serie[$i]["sceid"]."]\">
					  		".$taxa_reprovacao."
					  	</td>
					  	<td bgcolor=\"#f0f0f0\" id=\"taxa_abandono[".$serie[$i]["sceid"]."]\">
					  		".$taxa_abandono."
					  	</td>
					  </tr>";
		}
	}
		
		$div = ($totalMatInicial + $totalAdmitido - $totalTransferencia );
		if(($div != 0) && ( $div != '')) {
			@$totalTaxaAprovacao = str_replace('.',',',number_format(($totalAprovados / ($div) * 100)));
			@$totalTaxaReprovacao = str_replace('.',',',number_format(($totalReprovados / ($div ) * 100)));
			@$totaTaxaAbandono = str_replace('.',',',number_format(($totalAbandono / ($div ) * 100)));
		}
		else {
			$totalTaxaAprovacao = 0;
			$totalTaxaReprovacao = 0;
			$totaTaxaAbandono = 0;
		}
			
		echo "<tr height=\"30px\" align=\"center\">
					<td bgcolor=\"#DADADA\"><b>TOTAL</b></td>
					<td id=\"totalMatInicial\" bgcolor=\"#DADADA\">".$totalMatInicial."</td>
					<td id=\"totalAdmitido\" bgcolor=\"#DADADA\">".$totalAdmitido."</td>
					<td id=\"totalAbandono\" bgcolor=\"#DADADA\">".$totalAbandono."</td>
					<td id=\"totalTransferencia\" bgcolor=\"#DADADA\">".$totalTransferencia."</td>
					<td id=\"totalMatFinal\" bgcolor=\"#DADADA\">".$totalMatFinal."</td>
					<td id=\"totalAprovados\" bgcolor=\"#DADADA\">".$totalAprovados."</td>
					<td id=\"totalReprovados\" bgcolor=\"#DADADA\">".$totalReprovados."</td>
					<td id=\"totalTaxaAprovacao\" bgcolor=\"#DADADA\">".$totalTaxaAprovacao."%</td>
					<td id=\"totalTaxaReprovacao\" bgcolor=\"#DADADA\">".$totalTaxaReprovacao."%</td>
					<td id=\"totaTaxaAbandono\" bgcolor=\"#DADADA\">".$totaTaxaAbandono."%</td>
			  </tr>";
	?>
</tbody>

</table>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
 <? $cForm->montarbuttons("salva()");?>
</table>
</form>
<script language="javascript" type="text/javascript">
/*** Submete os dados do formulário ***/
function salva() {
 
			   var soma;
               var nTrue = 0;
               var nFalse = 0;
               var nf = 0;
               var nt = 0;
		for( var i = 24; i<= 27; i++ )
	       {

 somaMatFinal = (Number(document.getElementById('apaqtdmatriculainicial['+i+']').value) +
				    Number(document.getElementById('apaqtdadmitidosaposmarco['+i+']').value) -
				    Number(document.getElementById('apaqtdafastadostransferencia['+i+']').value));
	        
	               var apro  = document.getElementById('apaqtdaprovados['+i+']');
	               var repro = document.getElementById('apaqtdreprovados['+i+']');
	               var mat = document.getElementById('apaqtdmatriculafinal['+i+']');
	               var matIni = document.getElementById('apaqtdmatriculainicial['+i+']');
	               
//	               alert( apro );
//	               alert( repro );
//	               alert( mat);
//	               alert( matIni );
 	               
	               if( matIni.value != '' )
		           {
		               soma = (Number(apro.value) + Number(repro.value));
		                
		               if(soma != Number( mat.value ) ) 
		               { 
		               		  
		               		 nf = Number(nFalse + 1); 
							  	
			       	   } 
			       	   else
			       	   {
			       	   		 nt = Number(nTrue + 1);
			       	   }
			       	   
		       	   }			 
	       			
	        }//end for
		 
		if( nf == 0 )
		{	
			document.getElementById('formulario').submit();
			return true;
		}
		else
		{ 
			alert("A Soma dos Campos 'aprovado' e 'reprovado' não pode ser diferente da Matricula determinada.");
			return false;
		}
}
 


/*** Calcula e altera os totais e taxas correspondentes ***/
function alteraTaxa(id_campo,serie_campo,total_campo) {
	if(id_campo == 'apaqtdaprovados') {
		atualizarTotalizador(id_campo,total_campo);
		atualizaTaxaAprovacao(serie_campo);
	}
	if(id_campo == 'apaqtdreprovados') {
		atualizarTotalizador(id_campo,total_campo);
		atualizaTaxaReprovacao(serie_campo);
	}
	else {
		atualizaMatriculaFinal(serie_campo);
		atualizarTotalizador(id_campo,total_campo);
		atualizaTaxaAbandono(serie_campo);
		atualizaTaxaAprovacao(serie_campo);
		atualizaTaxaReprovacao(serie_campo);
	}
}

/*** Atualiza o campo 'Matrícula Final' correspondente e o totalizador destes campos. ***/
function atualizaMatriculaFinal(serie_campo) {
	var somaMatFinal = 0;
	
	somaMatFinal = (Number(document.getElementById('apaqtdmatriculainicial['+serie_campo+']').value) +
				    Number(document.getElementById('apaqtdadmitidosaposmarco['+serie_campo+']').value) -
 				    Number(document.getElementById('apaqtdafastadosabandono['+serie_campo+']').value) -
				    Number(document.getElementById('apaqtdafastadostransferencia['+serie_campo+']').value));
				   
	document.getElementById('apaqtdmatriculafinal['+serie_campo+']').value = somaMatFinal;
	
	atualizarTotalizador('apaqtdmatriculafinal','totalMatFinal');
}

/*** Atualiza o totalizador ***/
function atualizarTotalizador(id_campo,total_campo) {
	var soma = 0;
	
	for(var i=0; i<series.length; i++) {
		soma += Number(document.getElementById(id_campo + '['+series[i]+']').value);
	}
	document.getElementById(total_campo).innerHTML = soma;
}

/*** Atualiza a 'Taxa de Abandono' correspondente e o totalizador ***/
function atualizaTaxaAbandono(serie_campo) {
	var matFinal = document.getElementById('apaqtdmatriculafinal['+serie_campo+']').value;
	var abandono = Number(document.getElementById('apaqtdafastadosabandono['+serie_campo+']').value);
	var matInic  = Number(document.getElementById('apaqtdmatriculainicial['+serie_campo+']').value);
	var aprovados = Number(document.getElementById('apaqtdaprovados['+serie_campo+']').value);
	var qtdMarco = Number(document.getElementById('apaqtdadmitidosaposmarco['+serie_campo+']').value);
	var qtdTransf = Number(document.getElementById('apaqtdafastadostransferencia['+serie_campo+']').value);
	
	if((matFinal == 0) || (matFinal == "")) {
		var taxaAband = "0%";
	} else { 
		var taxaAband = Math.floor(Number(abandono / (matInic + qtdMarco - qtdTransf) * 100));
		taxaAband = taxaAband.toFixed(1).replace('.',',') + "%";
	}
	document.getElementById('taxa_abandono['+serie_campo+']').innerHTML = taxaAband;
	
	var totalMatFinal = document.getElementById('totalMatFinal').innerHTML;
	var totalAbandono = document.getElementById('totalAbandono').innerHTML;
	var totalAdmitido = document.getElementById('totalAdmitido').innerHTML;
	var totalTransferencia = document.getElementById('totalTransferencia').innerHTML;
	
	var totalDiv = 0;
	var totalDiv = (Number(totalMatInicial) + Number(totalAdmitido) + Number(totalTransferencia));
	
	if((totalDiv == 0) || (totalDiv == "")) {
		var totaTaxaAbandono = "0%";
	} else {
		var totaTaxaAbandono = ((totalAbandono / totalDiv) * 100);
		totaTaxaAbandono = totaTaxaAbandono.toFixed().replace('.',',') + "%";
	}
	document.getElementById('totaTaxaAbandono').innerHTML = totaTaxaAbandono;
}

/*** Atualiza a 'Taxa de Aprovação' correspondente e o totalizador ***/
function atualizaTaxaAprovacao(serie_campo) {
	var matFinal = document.getElementById('apaqtdmatriculafinal['+serie_campo+']').value;
	var matInic  = Number(document.getElementById('apaqtdmatriculainicial['+serie_campo+']').value);
	var aprovados = Number(document.getElementById('apaqtdaprovados['+serie_campo+']').value);
	var qtdMarco = Number(document.getElementById('apaqtdadmitidosaposmarco['+serie_campo+']').value);
	var qtdTransf = Number(document.getElementById('apaqtdafastadostransferencia['+serie_campo+']').value);
				   
	 
	if((matFinal == 0) || (matFinal == "")) {
		var taxaAprov = "0%";
	} else {
		//var taxaAprov = ((aprovados / matFinal) * 100);
		var taxaAprov = Math.floor(Number(aprovados / (matInic + qtdMarco - qtdTransf) * 100));
		taxaAprov = taxaAprov.toFixed(1).replace('.',',') + "%";
	}
	document.getElementById('taxa_aprovacao['+serie_campo+']').innerHTML = taxaAprov;
	
	var totalMatFinal = document.getElementById('totalMatFinal').innerHTML;
	var totalAprovados = document.getElementById('totalAprovados').innerHTML;
	var totalAdmitido = document.getElementById('totalAdmitido').innerHTML;
	var totalTransferencia = document.getElementById('totalTransferencia').innerHTML;
	
	var totalDiv = 0;
	var totalDiv = (Number(totalMatInicial) + Number(totalAdmitido) + Number(totalTransferencia));
		
	if((totalDiv == 0) || (totalDiv == "")) {
		var totalTaxaAprovacao = "0%";
	} else {
		var totalTaxaAprovacao = ((totalAprovados / totalDiv) * 100);
		totalTaxaAprovacao = totalTaxaAprovacao.toFixed().replace('.',',') + "%";
	}
	document.getElementById('totalTaxaAprovacao').innerHTML = totalTaxaAprovacao;
}

/*** Atualiza a 'Taxa de Reprovação' correspondente e o totalizador ***/
function atualizaTaxaReprovacao(serie_campo) {
	var matFinal = document.getElementById('apaqtdmatriculafinal['+serie_campo+']').value;
	var reprovados = Number(document.getElementById('apaqtdreprovados['+serie_campo+']').value);
	var matInic  = Number(document.getElementById('apaqtdmatriculainicial['+serie_campo+']').value);
	var aprovados = Number(document.getElementById('apaqtdaprovados['+serie_campo+']').value);
	var qtdMarco = Number(document.getElementById('apaqtdadmitidosaposmarco['+serie_campo+']').value);
	var qtdTransf = Number(document.getElementById('apaqtdafastadostransferencia['+serie_campo+']').value);
	
	if((matFinal == 0) || (matFinal == "")) {
		var taxaReprov = "0%";
	} else {
		//var taxaReprov = ((reprovados / matFinal) * 100);
		var taxaReprov = Math.floor(Number(reprovados / (matInic + qtdMarco - qtdTransf) * 100));
		taxaReprov = taxaReprov.toFixed(1).replace('.',',') + "%";
	}
	document.getElementById('taxa_reprovacao['+serie_campo+']').innerHTML = taxaReprov;
	
	var totalMatFinal = document.getElementById('totalMatFinal').innerHTML;
	var totalReprovados = document.getElementById('totalReprovados').innerHTML;
	var totalAdmitido = document.getElementById('totalAdmitido').innerHTML;
	var totalTransferencia = document.getElementById('totalTransferencia').innerHTML;
	
	var totalDiv = 0;
	var totalDiv = (Number(totalMatInicial) + Number(totalAdmitido) + Number(totalTransferencia));
			
	if((totalDiv == 0) || (totalDiv == "")) {
		var totalTaxaReprovacao = "0%";
	} else {
		var totalTaxaReprovacao = ((totalReprovados / totalDiv) * 100);
		totalTaxaReprovacao = totalTaxaReprovacao.toFixed().replace('.',',') + "%";
	}
	document.getElementById('totalTaxaReprovacao').innerHTML = totalTaxaReprovacao;
}
</script>
