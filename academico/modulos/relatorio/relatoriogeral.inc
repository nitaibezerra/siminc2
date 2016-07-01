<?

if($_POST['exibir'] == "relatorio") {
	function agrupar( $lista, $agrupadores ) {
	global $db;
	
	$existeProximo = count( $agrupadores ) > 0; 
	if ( $existeProximo == false ) {
		return array();
	}
	$campo = array_shift( $agrupadores );
	$novo = array();
	foreach ( $lista as $item )	{
		$chave = $item[$campo];
		
		if($chave) {
		if ( array_key_exists( $chave, $novo ) == false ){			
			$novo[$chave] = array(
				"orgaoid"   => $item["orgaoid"],
				$campo."id"   => $item[$campo."id"],
				"agrupador"   => $campo,
				"sub_itens"   => array()
			);
		}
		
		if ( $existeProximo ) {	
			array_push( $novo[$chave]["sub_itens"], $item );
		}
		
		}
	}
	if ( $existeProximo ) {
		foreach ( $novo as $chave => $dados )
		{
			$novo[$chave]["sub_itens"] = agrupar( $novo[$chave]["sub_itens"], $agrupadores );
		}
	}
	
	return $novo;
	}
	
	function exibir($dados = array(), $nivel = 0, $nivelid = 'nivel') {
		global $db,$filtro,$filtrofinal;
		
		echo "<table width='100%' cellpadding='0' cellspacing='0' border='0'>";
		;
		
		for($i = 0;$i < $nivel; $i++) {
			$espacamento .= '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
		}
		
		$i = 0;
		foreach($dados as $nome => $dado) {
			$pref = $nivelid.'_'.$i;
			$i++;
			unset($icomais);
			if(count($dado['sub_itens']) > 0 || $dado['agrupador'] == 'campus') {
				$icomais = "<img src='/imagens/menos.gif' border='0' title='fechar' id='btn_". $pref ."' onclick='abrearvore(\"". $pref ."\");'>";
			}
			$formato = (($i%2)?"bgcolor=\"#F7F7F7\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='#F7F7F7';\"":"bgcolor=\"\" onmouseover=\"this.bgColor='#ffffcc';\" onmouseout=\"this.bgColor='';\"");
			echo "<tr ". $formato .">";
			echo "<td>". $espacamento . $icomais ." ". $nome ."</td>";
			echo "</tr>";
			$filtrofinal[$dado['agrupador']] = $dado[$dado['agrupador'].'id'];
			if($dado['sub_itens']) {
				echo "<tr id='". $pref ."'><td>";
				exibir($dado['sub_itens'], $nivel+1, $pref);
				echo "</td></tr>";
			} else {
				echo "<tr  id='". $pref ."'><td>";
				
				unset($cabecalho);
				unset($cmpids);
				$dadositenssomados = array();
				$cmpids = filtrarcampus($filtrofinal);
				if(is_array($cmpids)) {
					foreach($cmpids as $cmpid) {
						if($filtro['ano']) {
							unset($paramselects);
							foreach($filtro['ano'] as $ano) {
								$paramselects[] = "CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."' AND cpi.cpitabnum=0) is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."' AND cpi.cpitabnum=0) END AS ano_".$ano;
							}
							$paramselects = implode(",",$paramselects);
						
							$sql = "SELECT itm.itmid,
								   '<strong>'||itm.itmdsc||'</strong>' as descricaoitem, tpimascara,
								   ". $paramselects ."
								   FROM academico.item itm 
								   LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
								   LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
								   WHERE itm.itmglobal = false AND tei.orgid = '". $dado['orgaoid'] ."' ".((count($filtro['campus']) > 0)?"AND ".implode(" AND ", $filtro['campus']):"")."
								   ORDER BY tei.teiordem";
							$dadositens = $db->carregar($sql);
							
							if($dadositens) {
								foreach($dadositens as $din) {
									$dadositenssomados[$din['itmid']]['descricaoitem'] = $din['descricaoitem'];
									$dadositenssomados[$din['itmid']]['tpimascara'] = $din['tpimascara'];
									foreach($filtro['ano'] as $ano) {
										$dadositenssomados[$din['itmid']]['ano_'.$ano] += $din['ano_'.$ano];
									}
								}
							}
						} else {
							echo "Não foi selecionado 'ano' para a pesquisa <br/>";
						}
					}
				}
				
				
				if(is_array($cmpids)) {
					foreach($cmpids as $cmpid) {
						if($filtro['ano']) {
							unset($paramselects);
							foreach($filtro['ano'] as $ano) {
								$paramselects[] = "CASE WHEN (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."' AND cpi.cpitabnum=1) is null THEN '' ELSE (SELECT (coalesce(cpitexto,'') || coalesce(cast(cpivalor as varchar),'')) FROM academico.campusitem cpi WHERE itm.itmid = cpi.itmid AND cpi.cpiano = '". $ano ."' AND cpi.cmpid = '". $cmpid['cmpid'] ."' AND cpi.cpitabnum=1) END AS ano_".$ano;
							}
							$paramselects = implode(",",$paramselects);
						
							$sql = "SELECT itm.itmid,
								   '<strong>'||itm.itmdsc||'</strong>' as descricaoitem, tpimascara,
								   ". $paramselects ."
								   FROM academico.item itm 
								   LEFT JOIN academico.tipoitem tpi ON tpi.tpiid = itm.tpiid 
								   LEFT JOIN academico.orgaoitem tei ON tei.itmid = itm.itmid 
								   WHERE itm.itmglobal = false AND tei.orgid = '". $dado['orgaoid'] ."' ".((count($filtro['campus']) > 0)?"AND ".implode(" AND ", $filtro['campus']):"")."
								   ORDER BY tei.teiordem";
							$dadositens = $db->carregar($sql);
							
							if($dadositens) {
								foreach($dadositens as $din) {
									$dadositenssomadosT[$din['itmid']]['descricaoitem'] = $din['descricaoitem'];
									$dadositenssomadosT[$din['itmid']]['tpimascara'] = $din['tpimascara'];
									foreach($filtro['ano'] as $ano) {
										$dadositenssomadosT[$din['itmid']]['ano_'.$ano] += $din['ano_'.$ano];
									}
								}
							}
						} else {
							echo "Não foi selecionado 'ano' para a pesquisa <br/>";
						}
					}
				}
				
				if(count($dadositenssomados) > 0) {
					print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
					
					print '<tr>';
					print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">Item</td>';
					foreach($filtro['ano'] as $ano) {
						print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">'.$ano.'</td>';
					}
					print '</tr>';
					foreach($dadositenssomados as $d) {
						print '<tr>';
						print '<td>'. $d['descricaoitem'] .'</td>';
						foreach($filtro['ano'] as $ano) {						
							$valor_item = $d['ano_'.$ano];		
							print '<td><input type="hidden" name="gravacaocampo_" onkeyup="this.value=mascaraglobal(\''.$d['tpimascara'].'\', this.value)"; value="'.$valor_item.'"></td>';
						}
						print '</tr>';
					}
					
					print '</table>';
				}
				
				if(count($dadositenssomadosT) > 0) {
					print '<br/><br/><table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
					
					print '<tr>';
					print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">Item (TOTAL)</td>';
					foreach($filtro['ano'] as $ano) {
						print '<td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor=\'#c0c0c0\';" onmouseout="this.bgColor=\'\';">'.$ano.'</td>';
					}
					print '</tr>';
					foreach($dadositenssomadosT as $d) {
						print '<tr>';
						print '<td>'. $d['descricaoitem'] .'</td>';
						foreach($filtro['ano'] as $ano) {						
							$valor_item = $d['ano_'.$ano];		
							print '<td><input type="hidden" name="gravacaocampo_" onkeyup="this.value=mascaraglobal(\''.$d['tpimascara'].'\', this.value)"; value="'.$valor_item.'"></td>';
						}
						print '</tr>';
					}
					
					print '</table>';
				}
				
				echo "</td></tr>";
			}
		}
		echo "</table>";
	}
	
	// Analisando o filtro geral
	if($_POST['orgid'][0]) {
		$filtro['geral'][] = "teo.orgid IN('".implode("','",$_POST['orgid'])."')"; 
	}
	if($_POST['estuf'][0]) {
		$filtro['geral'][] = "en.estuf IN('".implode("','",$_POST['estuf'])."')"; 
	}
	if($_POST['unidades'][0]) {
		foreach($_POST['unidades'] as $unidade) {
			$detalhesunidade = $unidade;
			$filuni[] = "(e.entid = '".$detalhesunidade[0]."')";
		}
		$filtro['geral'][] = "(". implode(" OR ", $filuni) .")";
	}
	// Analisando o filtro dos dados campus
	if($_POST['itmid'][0]) {
		$filtro['campus'][] = "itm.itmid IN('".implode("','",$_POST['itmid'])."')"; 
	}
	if($_POST['ano'][0]) {
		$filtro['ano'] = $_POST['ano'];
	}
	$sql = "SELECT en.estuf as uf, en.estuf as ufid, teo.orgdesc as orgao, 
					teo.orgid as orgaoid, e.entnome as campus, ea.entid as unidadeid, 
					ea.entnome as unidade, cam.cmpid as campusid    			
 			FROM academico.campus cam 
 			LEFT JOIN entidade.entidade e ON e.entid = cam.entid 
 			LEFT JOIN entidade.funcaoentidade fen ON fen.entid = e.entid 
 			LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
			LEFT JOIN entidade.entidade ea ON ea.entid = fea.entid 
			LEFT JOIN entidade.funcaoentidade fen2 ON fen2.entid = ea.entid 			
			LEFT JOIN academico.orgaouo teu ON teu.funid = fen2.funid	
			LEFT JOIN academico.orgao teo ON teu.orgid = teo.orgid
			INNER JOIN entidade.endereco en ON e.entid = en.entid			
 			".((count($filtro['geral']) > 0)?"WHERE ".implode(" AND ", $filtro['geral']):"")." ORDER BY uf, orgao, campus, unidade";
	$dados = $db->carregar( $sql );
	$dados = $dados ? $dados : array();
	$dadosagrupados = agrupar($dados, $_REQUEST['agrupador']);
	$titulo_modulo = "Sistema de Informações Gerenciais";
	monta_titulo( $titulo_modulo,'Relatório Geral');
	
	?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script>
	function abrearvore(pref) {
		var img = document.getElementById("btn_"+pref);
		if(img.title == 'abrir') {
			img.title = 'fechar';
    		img.src = '/imagens/menos.gif';
	    	var acao = '';
    	} else {
    		img.title = 'abrir';
	    	img.src = '/imagens/mais.gif';
    		var acao = 'none';
	    }
   		document.getElementById(pref).style.display = acao;
	}
	</script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<table class="tabela" align="center" cellspacing="1" cellpadding="3">		
<tr>
<td>
<form method='post' id='formulario2'>
	<?
	exibir($dadosagrupados);
	?>
</form>
</td>
</tr>
<script>
	aplicarmascara();
	function aplicarmascara() {
		var form = document.getElementById('formulario2');
		for (var j=0; j < form.length; j++){
			if(form[j].value != "" && form[j].type == "hidden" && form[j].name.substr(0,14) == "gravacaocampo_") {
				form[j].onkeyup();
				form[j].parentNode.innerHTML = form[j].parentNode.innerHTML+form[j].value;
				form[j].parentNode.style.textAlign = 'right';
			}
		}
	}
</script>

<tr>
<td colspan="2" align="right">
<input type="button" value="Voltar" onclick="window.close()">

<form action='relatoriogeralxls2.php' method='post' name='formulario'>		
		
		<?
		// gerando os inputs hidden para cada item de seleção
		if($_POST['agrupador']) {
			foreach($_POST['agrupador'] as $agrupador) {
			echo("<input type='hidden' name='agrupador[]' value='".$agrupador."'/>");		
			}
		}	
		if($_POST['orgid']) {
			foreach($_POST['orgid'] as $orgid) {
			echo("<input type='hidden' name='orgid[]' value='".$orgid."'/>");		
			}
		}	
		if($_POST['estuf']) {
			foreach($_POST['estuf'] as $estuf) {
			echo("<input type='hidden' name='estuf[]' value='".$estuf."'/>");		
			}
		}
		
		if($_POST['unidades']) {
			foreach($_POST['unidades'] as $unidades) {
			echo("<input type='hidden' name='unidades[]' value='".$unidades."'/>");		
			}
		}
		if($_POST['itmid']) {
			foreach($_POST['itmid'] as $itmid) {
			echo("<input type='hidden' name='itmid[]' value='".$itmid."'/>");		
			}
		}
		if($_POST['ano']) {
			foreach($_POST['ano'] as $ano) {
			echo("<input type='hidden' name='ano[]' value='".$ano."'/>");		
			}
		}
		
		?>
		<!-- <input type="submit" value="XLS">  -->
</form></td>
</tr>
</table>
<?
	exit;
}

session_start();
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php'; 
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";
echo "<br/>";
$titulo_modulo = "Sistema de Informações Gerenciais";
monta_titulo( $titulo_modulo,'Relatório Geral');
?>
<script>

function abrirPopup(){
	window.open('', 'abrirPopup', 'toolbar=no,location=no,directories=no,status=no,menubar=no,scrollbars=yes,resizable=yes,copyhistory=yes,width=800,height=600');	
}


function abrepopupUnidade(){
	janela = window.open('combo_unidades.php','Unidades','width=400,height=400,scrollbars=1,top=200,left=480');
	janela.focus();
}

function exibirRelatorio() {
	var formulario = document.formulario;
	/*
	 * Verifica se pelo menos um tipo de ensino foi selecionado
	 */
	var orgaomarcado = false
	for(i=0;i<formulario.elements['orgid[]'].length;i++) {
		if(formulario.elements['orgid[]'][i].checked) {
			orgaomarcado = true;
		}
	}
	if(!orgaomarcado) {
		alert('Escolha ao menos um tipo de ensino');
		return false;
	}
	/*
	 * Verifica se pelo menos um ano foi selecionado
	 */
	var anomarcado = false
	for(i=0;i<formulario.elements['ano[]'].length;i++) {
		if(formulario.elements['ano[]'][i].checked) {
			anomarcado = true;
		}
	}
	if(!anomarcado) {
		alert('Escolha ao menos um ano');
		return false;
	}

		
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return false;
	}
	selectAllOptions( agrupador );
	//Filtros
	selectAllOptions( document.formulario.estuf );
	selectAllOptions( document.formulario.itmid );
	selectAllOptions( document.formulario.unidades );	
	abrirPopup();
}


</script>
<form action="" method="post" name="formulario" onsubmit="return exibirRelatorio();" target="abrirPopup">
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Agrupadores
			</td>
			<td width="80%">
<?
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/rarrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <!--
                <img src="../imagens/rarrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );"/><br/>
                <img src="../imagens/larrow_all.gif" style="padding: 5px" onClick="moveAllOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, ''); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
                -->
                <img src="../imagens/larrow_one.gif" style="padding: 5px" onClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );"/><br/>
            </td>
            <td>
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 160px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
            </td>
            <td>
                <img src="../imagens/uarrow.gif" style="padding: 5px" onClick="subir( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
                <img src="../imagens/darrow.gif" style="padding: 5px" onClick="descer( document.getElementById( '{NOME_DESTINO}' ) );"/><br/>
            </td>
        </tr>
    </table>
    <script type="text/javascript" language="javascript">
        limitarQuantidade( document.getElementById( '{NOME_DESTINO}' ), {QUANTIDADE_DESTINO} );
        limitarQuantidade( document.getElementById( '{NOME_ORIGEM}' ), {QUANTIDADE_ORIGEM} );
        {POVOAR_ORIGEM}
        {POVOAR_DESTINO}
        sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );
    </script>
EOF;
$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
$destino = array();
$origem = array(  	   'uf' => array(
							'codigo'    => 'uf',
							'descricao' => 'UF'
						),
						
						'orgao' => array(
							'codigo'    => 'orgao',
							'descricao' => 'Tipo de ensino'
						),
						'unidade' => array(
							'codigo'    => 'unidade',
							'descricao' => 'Unidade'
						),
						'campus' => array(
							'codigo'    => 'campus',
							'descricao' => 'Campus'
						)
				);
					
// exibe agrupador

$agrupador->setOrigem( 'naoAgrupador', null, $origem );
$agrupador->setDestino( 'agrupador', null, $destino );
$agrupador->exibir();


?>
</td>
</tr>
		<tr>
			<td colspan="2">Filtros</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Tipo de ensino
			</td>
			<td>
				<?php
				$sqlComboorgao = "SELECT orgid as codigo, orgdesc as descricao from academico.orgao order by orgdesc";
				$tipoens = $db->carregar($sqlComboorgao);
				if($tipoens) {
					foreach($tipoens as $tp) {
						echo "<input type='checkbox' name='orgid[]' value='".$tp['codigo']."'> ".$tp['descricao']." ";
					}
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Unidades da Federação
			</td>
			<td>
				<?php
				$sqlComboEstado = "
					select
						estuf as codigo,
						estdescricao as descricao
					from territorios.estado
					order by
						estdescricao
				";
				combo_popup( "estuf", $sqlComboEstado, "Estado", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
    <tr>
        <td class="SubTituloDireita" valign="top">Unidades</td>
        <td>
       <select multiple="multiple" size="5" name="unidades[]" 
        id="unidades"  
        ondblclick="abrepopupUnidade();"  
        class="CampoEstilo" style="width:400px;" >
        <option value="">Duplo clique para selecionar da lista</option>
        </select>
        </td>
    </tr>   

		<tr>
			<td class="SubTituloDireita" valign="top">
				Itens
			</td>
			<td>
				<?php
				
				
				$sqlComboItem = "
					select
						itmid as codigo,
						itmdsc as descricao
					from academico.item 
					where itmstatus = 'A' AND itmglobal = false
					order by
						descricao
				";
				combo_popup( "itmid", $sqlComboItem, "Item", "400x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">
				Ano
			</td>
			<td>
				<?php
				$anos = array(2005,2006,2007,2008,2009,2010,2011,2012);
				foreach($anos as $ano) {
					echo "<input type=\"checkbox\" name=\"ano[]\" value=". $ano ."> ".$ano."&nbsp;&nbsp;&nbsp;";
				}
				?>
			</td>
		</tr>

<tr>
<td colspan="2"><input type="submit" value="Exibir"><input type="hidden" name="exibir" value="relatorio"></td>
</tr>
</table>
</form>