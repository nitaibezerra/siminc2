<?php

switch($_REQUEST['buscar']) {
	case '1':
		include "relatoriogeral_resultado.inc";
		exit;
	case '2':
		if($_POST['filano'])
			$_SESSION['rehuf_var']['filano']=$_POST['filano'];
		if($_POST['entid'])
			$_SESSION['rehuf_var']['hospitalentid']=$_POST['entid'];
		if($_POST['filper'])
			$_SESSION['rehuf_var']['filper']=$_POST['filper'];
		
		if($_REQUEST['agrupador'] && (!$_REQUEST['gitid'] || is_array($_REQUEST['gitid']))) {
			echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../includes/Estilo.css\"/>
				  <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>";
			
			foreach($_REQUEST['agrupador'] as $tblid) {
				// Pegando dados da TABELA (pai de toda a estrutura)
				$tabela = $db->pegaLinha("SELECT tabtid, tabtdsc, tabanoini, tabanofim FROM rehuf.tabela WHERE tabtid ='".$tblid."'");
				echo "<table class=\"tabela\" align=\"center\" bgcolor=\"#f5f5f5\" cellspacing=\"1\" cellpadding=\"3\">";
				echo "<tr><td class='SubTituloCentro' colspan='2'>".$tabela['tabtdsc']."</td></tr>";
				$grupoitem = (array) $db->carregar("SELECT * FROM rehuf.grupoitem WHERE tabtid = '". $tabela['tabtid'] ."' ORDER BY gitordem");
				foreach($grupoitem as $grp) {
					echo "<tr><td class='SubTituloDireita' width='50%'>".$grp['gitdsc']."</td><td><form action='' method='post' name='formulario'><input type='hidden' name='buscar' value='2'><input type='hidden' name='agrupador[]' value='".$tblid."'><input type='hidden' name='gitid' value='".$grp['gitid']."'><input type='submit' value='Clique aqui'></form></td></tr>";
				}
				echo "</table>";

			}
			exit;
		}
		include "relatoriogeralexcel_resultado.inc";
		exit;
	case '3':
		include "relatoriogeralagrupado_resultado.inc";
		exit;
		
}

include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( "Relatório Geral - REHUF", "" );

$_SESSION['rehuf_var']['filano']=array();

?>
<script type="text/javascript">
function exibirRelatorio() {
	var formulario = document.formulario;
	if(formulario.agrupardados.checked) {
		formulario.buscar.value='3';
	} else {
		formulario.buscar.value='1';
	}
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return;
	}
	// verifica se tem algum ano selecionado
	if (document.formulario.elements['filano[]'].length > 0) {
		var anomarcado = false;
		for(i=0;i<document.formulario.elements['filano[]'].length;i++) {
			if(document.formulario.elements['filano[]'][i].checked) {
				anomarcado = true;
			}
		}
		if(!anomarcado) {
			alert( 'Escolha ao menos um ano.' );
			return false;
		}
	}

	selectAllOptions( agrupador );
	selectAllOptions( document.getElementById( 'entid' ) );
	selectAllOptions( document.getElementById( 'gitid' ) );

	// submete formulario
	formulario.target = 'relatoriogeralrehuf';
	var janela = window.open( '', 'relatoriogeralrehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}
function exibirRelatorioExcel() {
	var formulario = document.formulario;
	formulario.buscar.value='2';
	// verifica se tem algum agrupador selecionado
	agrupador = document.getElementById( 'agrupador' );
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return;
	}
	selectAllOptions( agrupador );
	selectAllOptions( document.getElementById( 'entid' ) );
	selectAllOptions( document.getElementById( 'gitid' ) );

	// submete formulario
	formulario.target = 'relatoriogeralrehuf';
	var janela = window.open( '', 'relatoriogeralrehuf', 'width=900,height=645,status=1,menubar=1,toolbar=0,resizable=0,scrollbars=1' );
	formulario.submit();
	janela.focus();
}

function verificarPeriodosAno(obj) {

	if(obj.checked) {
		if(document.getElementById("periodo_"+obj.value)) {
			document.getElementById("periodo_"+obj.value).style.display = '';
		}
	} else {
		if(document.getElementById("periodo_"+obj.value)) {
			document.getElementById("periodo_"+obj.value).style.display = 'none';
		}
	}
	
}
</script>

<form action="" method="post" name="formulario">
	<input type="hidden" name="buscar" value="1"/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">Tabelas :</td>
			<td width="80%">
			<?
$agrupadorHtml =
<<<EOF
    <table>
        <tr valign="middle">
            <td>
                <select id="{NOME_ORIGEM}" name="{NOME_ORIGEM}[]" multiple="multiple" size="4" style="width: 350px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_ORIGEM}' ), document.getElementById( '{NOME_DESTINO}' ), true, '' );" class="combo campoEstilo"></select>
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
                <select id="{NOME_DESTINO}" name="{NOME_DESTINO}[]" multiple="multiple" size="4" style="width: 350px; height: 70px;" onDblClick="moveSelectedOptions( document.getElementById( '{NOME_DESTINO}' ), document.getElementById( '{NOME_ORIGEM}' ), true, '' ); sortSelect( document.getElementById( '{NOME_ORIGEM}' ) );" class="combo campoEstilo"></select>
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
					// inicia agrupador
					$agrupador = new Agrupador( 'formulario', $agrupadorHtml );
					$destino = array();
					$sql = "SELECT tabtid AS codigo, tabtdsc AS descricao FROM rehuf.tabela";
					$tabelas = $db->carregar($sql);
					
					// exibe agrupador
					$agrupador->setOrigem( 'naoAgrupador', null, $tabelas );
					$agrupador->setDestino( 'agrupador', null, $destino );
					$agrupador->exibir();
					
				?>
			</td></tr>
        <tr><td>Filtros</td></tr>		
		<tr>
			<td class="SubTituloDireita" valign="top">Hospitais :</td>
			<td>
			<?
			$permissoes = verificaPerfilRehuf();
			if(count($permissoes['verhospitais']) > 0) {
				$sqlHospitais = "SELECT ent.entid as codigo,  ena.entsig || ' - ' || ent.entnome as descricao FROM entidade.entidade ent 
				 				 LEFT JOIN rehuf.estruturaunidade esu ON esu.entid = ent.entid 
								 LEFT JOIN entidade.funcaoentidade fen ON fen.entid = ent.entid 
								 LEFT JOIN entidade.funentassoc fea ON fea.fueid = fen.fueid 
								 LEFT JOIN entidade.entidade ena ON ena.entid = fea.entid 
								 WHERE fen.funid = '". HOSPITALUNIV ."' AND (esu.esuindexibicao IS NULL OR esu.esuindexibicao = true) AND ent.entid IN('".implode("','",$permissoes['verhospitais'])."') 
								 ORDER BY ena.entsig";
				combo_popup( "entid", $sqlHospitais, "Hospitais", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
				
			} else {
				echo "Seu perfil não possui hospitais";	
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">GRIDs :</td>
			<td>
			<?
			$sqlGrupos = "SELECT gitid as codigo, tabtdsc|| ' - ' ||gitdsc as descricao FROM rehuf.grupoitem g 
						  INNER JOIN rehuf.tabela t ON t.tabtid = g.tabtid 
						  ORDER BY t.tabtdsc, g.gitdsc";
			combo_popup( "gitid", $sqlGrupos, "GRIDs", "192x400", 0, array(), "", "S", false, false, 5, 400 );				
				
			?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita">Anos :</td>
			<td>
			<?
				foreach($_ANOS as $ano) {
					echo "<input type=\"checkbox\" onclick=verificarPeriodosAno(this); name=\"filano[]\" value=\"".$ano."\"> ".$ano;
				}
			?>
			</td>
		</tr>
		<?
		
		foreach($_ANOS as $ano) {
			
			$sql = "SELECT perdsc, perano FROM rehuf.periodogrupoitem 
					WHERE perano='".$ano."'
					GROUP BY perdsc, perano 
					ORDER BY perano, perdsc";
			
			$perdsc_s = $db->carregar($sql);
			
			unset($input_PER);
			if($perdsc_s[0]) {
				foreach($perdsc_s as $p) {
					$input_PER .= "<input type=\"checkbox\" name=\"filper[]\" value=\"".$p['perdsc']."\"> ".$p['perdsc'];
				}
			}
			
			if($input_PER) {
				
				echo "<tr id=periodo_".$ano." style=display:none>
						<td class=\"SubTituloDireita\">Filtro por períodos <b>(".$ano.")</b></td>
						<td>".$input_PER."</td>
					  </tr>";
					
			}
		}
		?>
		<tr>
			<td class="SubTituloDireita">Deseja agrupar os dados ?</td>
			<td><input type="checkbox" name="agrupardados" value="1" onclick="if(this.checked){document.getElementById('gerarexcelbtn').disabled=true;}else{document.getElementById('gerarexcelbtn').disabled=false;}"></td>
		</tr>
	    <tr>
		<td class="SubTituloDireita" valign="top">&nbsp;</td>
		<td class="SubTituloDireita" style="text-align:left;"><input type="button" name="filtrar" value="Visualizar" onclick="exibirRelatorio();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/> <input type="button" name="filtrar" value="Gerar Excel" title="Caso a pesquisa retorna mais de 65.536 registros, o relatório deverá ser aberto no excel versão 2010 ou superior." id="gerarexcelbtn" onclick="exibirRelatorioExcel();" <? echo ((count($permissoes['verhospitais']) > 0)?"":"disabled"); ?>/></td>
	</tr>
</table>