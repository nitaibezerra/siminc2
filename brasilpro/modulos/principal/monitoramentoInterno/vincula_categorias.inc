<?php

if( $_POST['req'] == 'excluir' ){
	
	global $db;
	
	$testa = 0;
	
	$sql = "SELECT count(mosid)
			FROM cte.monitoramentosubacoes
			WHERE sbaid = ".$_POST['sbaid'];
	
	$testa = $db->pegaUm($sql);
	
	if( $testa == 0 ){
		$sql = "DELETE FROM cte.categoriasubacao
				WHERE catid = ".$_POST['catid']." AND
					  sbaid = ".$_POST['sbaid'];
		echo "Vínculo excluído.";
	}else{
		echo "Não é possível excluir este vínculo, pois existe monitoramento em andamento para esta subação.";
	}
	
	$db->executar($sql);
	$db->commit();
	die();
}

if( $_POST['req'] == 'buscarSub' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	if( $_POST['uf'] == '' ){
		echo "<select disabled >
				<option value=''>Escolha um estado.</option>
			  </select>";
		die();
	}
	
	// DECLARAÇÕES DE VARIAVEIS
	global $db;
	$complementoFrom = " ";
	
	// MONTA SQL DE ACORDO COM A ESCOLHA DO USUÁRIOS PELOS COMBOS DE FORMA DE EXECUÇÃO
	if($_POST['frm'] == '17'){ // forma de execução assistencia financeira
		$complementoFrom 	= " INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = s.sbaid AND cast(spt.sptano AS varchar) = '{$_POST['ano']}' AND spt.ssuid = 7";
		$frmid 	= 17;
	}else if($_POST['frm'] == '13'){ // forma de execução ações direta do estado
		$complementoFrom 	= " INNER JOIN cte.subacaoparecertecnico    sp  ON sp.sbaid    = s.sbaid AND sp.ssuid = 3 AND sp.sptano = ".$_POST['ano'];
		;
		$frmid 	= 13;
	}else if($_POST['frm'] == '15'){ // forma de execução assistencia técnica
		$complementoFrom 	= " INNER JOIN cte.subacaoparecertecnico    sp  ON sp.sbaid    = s.sbaid AND sp.ssuid = 3 AND sp.sptano = ".$_POST['ano'];
		;
		$frmid 	= 15;
	}if($_POST['frm'] == '21'){ // forma de execução "Assistência Financeira - Obra"
		$complementoFrom 	= " INNER JOIN cte.subacaoparecertecnico      spt ON spt.sbaid = s.sbaid AND cast(spt.sptano AS varchar) = '{$_POST['ano']}' AND spt.ssuid = 7";
		$frmid 	= 21;
	}
	
	$sql = "SELECT 	s.sbaid  as codigo,
				(dim.dimcod || '.' || 
				 are.ardcod || '.' || 
				 ind.indcod || ' ' ||	
				 SUBSTRING(s.sbadsc, 1, 150)) as descricao
			FROM cte.subacaoindicador s
				INNER JOIN cte.acaoindicador         a   ON s.aciid     = a.aciid
				INNER JOIN cte.pontuacao             p   ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
				INNER JOIN cte.indicador             ind ON ind.indid   = p.indid
				INNER JOIN cte.areadimensao          are ON are.ardid   = ind.ardid
				INNER JOIN cte.dimensao              dim ON dim.dimid   = are.dimid
				INNER JOIN cte.instrumentounidade    iu  ON iu.inuid    = p.inuid 
				".$complementoFrom."
			WHERE iu.estuf = '".$_POST['uf']."' 
			AND s.frmid = ".$frmid."
			AND iu.itrid = 3
			AND s.sbaid NOT IN (SELECT sbaid FROM cte.categoriasubacao)
			AND s.sbaidpai is null
			ORDER BY descricao ";
	$db->monta_combo( 'subacao', $sql, 'S', 'Selecione', 'resumoSub', '','','200','S','subacao' );
	
	die();
}

if( $_POST['req'] == 'vinculados' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;
	
	$sql = "SELECT 
				'<img onclick=\"return excluirVinculo('|| c.catid ||', '|| s.sbaid ||', '|| CASE WHEN ps.pssano IS NULL THEN 0 ELSE ps.pssano END ||' )\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acoes,
				(dim.dimcod || '.' || 
				 are.ardcod || '.' || 
				 ind.indcod || ' ' ||	
				 SUBSTRING(s.sbadsc, 1, 150)) as subacao,
				 c.cordsc as categoria,
				 CASE WHEN cs.casexibir 
				 	THEN '<div id=\"exibir'|| cs.casid ||'\" onclick=\"exibe('|| cs.casid ||')\" style=\"cursor:pointer\"><input type=\"hidden\" id=\"'|| cs.casid ||'\" value=\"0\"/> <a style=\"cursor:pointer\">Exibir</a></div>' 
				 	ELSE '<div id=\"exibir'|| cs.casid ||'\" onclick=\"exibe('|| cs.casid ||')\" style=\"cursor:pointer\"><input type=\"hidden\" id=\"'|| cs.casid ||'\" value=\"1\"/> <a style=\"cursor:pointer\">Ocultar</a></div>' 
				 END as exibir
			FROM cte.subacaoindicador s
			INNER JOIN cte.acaoindicador         a ON s.aciid     = a.aciid
			INNER JOIN cte.pontuacao             p ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
			INNER JOIN cte.indicador           ind ON ind.indid   = p.indid
			INNER JOIN cte.areadimensao        are ON are.ardid   = ind.ardid
			INNER JOIN cte.dimensao            dim ON dim.dimid   = are.dimid
			INNER JOIN cte.instrumentounidade   iu ON iu.inuid    = p.inuid 
			INNER JOIN cte.categoriasubacao     cs ON cs.sbaid    = s.sbaid
			INNER JOIN cte.categoria            c  ON c.catid     = cs.catid
			INNER JOIN cte.fasescategoria       fc ON fc.catid    = c.catid
			LEFT JOIN cte.projetosapesubacao    ps ON ps.sbaid    = s.sbaid
			WHERE 
				iu.estuf = '".$_POST['uf']."' 
				AND s.frmid =".$_POST['frm']." 
				AND iu.itrid = 3
			GROUP BY 
				subacao, categoria, c.catid, s.sbaid,ps.pssano,cs.casexibir,cs.casid";
	
	echo "
		<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">
			<tr>
				<td align=\"center\" gbcolor=\"#dcdcdc\" colspan=\"2\" bgcolor=\"#dcdcdc\" >
					<label class=\"SubTitulo\" style=\"color: rgb(0, 0, 0);\">Subações Vinculadas</label>
				</td>
			</tr>
			<tr>
				<td>";
	
	$db->monta_lista( $sql, array('Ações','Subação','Categoria', 'Exibição'), 99999999, 99999999, 'N','','','vinculacoes',array('5%','55%','40%'),array('center','left','left') );
//	$db->monta_lista_simples( $sql, array('Subação','Categoria'), 50, 1000, 'N', '100%','N' );
			
	echo "</td></tr></table>";
	
	die();
}

if( $_POST['req'] == 'resumoSub' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;
	
	$sql = "SELECT
				ind.inddsc as indicador,
				(dim.dimcod || '.' || 
				 are.ardcod || '.' || 
				 ind.indcod || ' ' ||
				s.sbadsc) as descricao,
				s.sbastgmpl as estrategia
			FROM
				cte.subacaoindicador s
			INNER JOIN cte.programa        p ON p.prgid   = s.prgid
			INNER JOIN cte.acaoindicador aci ON aci.aciid = s.aciid
			INNER JOIN cte.pontuacao     pto ON pto.ptoid = aci.ptoid AND pto.ptostatus = 'A'
			INNER JOIN cte.indicador     ind ON ind.indid = pto.indid
			INNER JOIN cte.areadimensao  are ON are.ardid = ind.ardid
			INNER JOIN cte.dimensao      dim ON dim.dimid = are.dimid
			WHERE
			    s.sbaid = ".$_POST['sbaid']." AND
			    s.frmid = ".$_POST['frm'];
	
	$dados = $db->pegaLinha($sql);
	
	echo "
		<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">
			<tr>
				<td align=\"center\" gbcolor=\"#dcdcdc\" colspan=\"2\" bgcolor=\"#dcdcdc\" >
					<label class=\"SubTitulo\" style=\"color: rgb(0, 0, 0);\">Resumo da Subação</label>
				</td>
			</tr>
			<tr>
				<td>Indicador da Subação:</td>
				<td>".$dados['indicador']."</td>
			</tr>
			<tr>
				<td>Descrição da Subação:</td>
				<td>".$dados['descricao']."</td>
			</tr>
			<tr>
				<td>Estratégia da Implementação:</td>
				<td>".$dados['estrategia']."</td>
			</tr>
		</table>
		";
	die();
}

if( $_POST['req'] == 'resumoCat' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;
	
	$sql = "SELECT cordsc
			FROM cte.categoria
			WHERE catid = ".$_POST['catid'];
	
	$dados = $db->pegaLinha($sql);
	
	$sqlFase = "SELECT 
					COALESCE(facordem::CHARACTER,'') || 'ª Fase: '|| facdsc as descricao,
					facobservacao as observacao
				FROM cte.fasescategoria
				WHERE catid = ".$_POST['catid'];
	
	echo "
		<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">
			<tr>
				<td  align=\"center\" gbcolor=\"#dcdcdc\" colspan=\"2\" bgcolor=\"#dcdcdc\">
					<label class=\"SubTitulo\" style=\"color: rgb(0, 0, 0);\">Resumo da Categoria</label>
				</td>
			</tr>
			<tr>
				<td width=\"10%\" class=\"SubTituloDireita\">Categoria:</td>
				<td>".$dados['cordsc']."</td>
			</tr>
			<tr>
				<td width=\"10%\"></td>
				<td>";
					$db->monta_lista_simples( $sqlFase, array('Fases','Descrição'), 99999999, 99999999, 'N', '100%','N' );
	echo 	   "</td>		
			</tr>
		</table>
		";
	die();
}

if( $_POST['req'] == 'vincular' ){
	
	global $db;
	
	$sql = "INSERT INTO cte.categoriasubacao(catid,sbaid,casexibir)
			VALUES (".$_POST['categoria'].",".$_POST['subacao'].",".$_POST['casexibir'].")";
	$db->executar($sql);
	$db->commit();
	echo "Subação vinculada com sucesso.";
	die();
}

if( $_POST['req'] == 'altexibe' ){
	
	global $db;
	$exibe = $_POST['exibe'] == 1 ? 'TRUE' : 'FALSE';
	$sql = "UPDATE cte.categoriasubacao SET
				casexibir = ".$exibe."
			WHERE
				casid = {$_POST['casid']}";
	$db->executar($sql);
	$db->commit();
	die();
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$res = array( 0 =>
					array ("descricao" => "Pesquisa de Categorias",
							    "id"        => "1",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_categorias&acao=A"
							   )
			);

if( $db->testa_superuser() ){	
		array_push($res,
					array ("descricao" => "Cadastrar Categorias",
							    "id"        => "2",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_categorias&acao=A"
							   ),
		   			array ("descricao" => "Vincular Categorias nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_categorias&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_categorias&acao=A");
//cte_montaTitulo( 'Vincular Categorias nas Subações', '&nbsp;' );
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="2" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Vincular categorias nas subações</label>
		</td>
	</tr>
	<tr>
		<td style="" align="center" bgcolor="#e9e9e9" colspan="2">
			&nbsp;
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			UF da Subação:
		</td>
		<td>
			<?php 
				$unidades = "'" . implode( "','", cte_pegarUfsPermitidas() ) . "'";
	            $sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e where e.estuf in ({$unidades}) order by e.estdescricao asc";
	            $db->monta_combo( 'estuf', $sql, 'S', 'Selecione', 'carregaSubacao', '' ,'','200','S','estuf', '', $_REQUEST["uf"] );
            ?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Forma de Execução:
		</td>
		<td>
			<?php 
				$sql = "SELECT frmid as codigo, frmdsc as descricao FROM cte.formaexecucao WHERE frmbrasilpro = true AND frmtipo = 'E' AND frmid not in (19,21) ORDER BY frmdsc";
	            $db->monta_combo( 'frm', $sql, 'S', 'Selecione', 'carregaSubacao', '','','200','S','frm','',$_REQUEST['frm'] );
            ?>
		</td>
	</tr>
	<tr id="anosubacoes" >
		<td class='SubTituloDireita' width="10%">
			Ano:
		</td>
		<td>
			<?php 
			$_REQUEST['ano'] = $_REQUEST['ano'] ? $_REQUEST['ano'] : ''; 
			?>
			<select id="ano" name="ano" onchange="carregaSubacao()" >
				<option value="<?=$_REQUEST['ano']?>"><?=$_REQUEST['ano'] ?></option>
				<?php for($x=2008;$x<=date('Y');$x++): ?>
					<option value="<?php echo $x ?>"><?php echo $x ?></option>
				<?php endfor; ?>
			</select>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Subação:
		</td>
		<td>
			<div id='divSub'>
				<?php 
					if( $_POST['uf'] == '' ){
				?>
					<select disabled id="subacao" name="subacao">
						<option value="">Escolha um estado.</option>
					</select>
				<?php 					
					}
				 ?>
			</div>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Categoria:
		</td>
		<td>
			<?php
				$sql = "SELECT 
							CAT.catid   as codigo, 
							cat.cordsc  as descricao 
						FROM cte.categoria cat
						ORDER BY cat.cordsc asc";
	            $db->monta_combo( 'categoria', $sql, 'S', 'Selecione', 'resumoCat', '','','200','S','categoria' );
			?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Exibição em Monitoramento:
		</td>
		<td>
			<?php
				$sql = "SELECT 
							'true'  as codigo, 
							'Exibir'  as descricao 
						UNION ALL
						SELECT 
							'false'  as codigo, 
							'Ocultar'  as descricao ";
	            $db->monta_combo( 'casexibir', $sql, 'S', 'Selecione', '', '','','200','S','casexibir','','true' );
			?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
		</td>
		<td>
			<input type="button" id="btnVincular" name="btnVincular" value="Vincular" onclick="vinculaSubCat()"/>
		</td>
	</tr>
	<tr>
		<td width="10%">
		</td>
		<td align='left'>
			<div id='resumoSub' align='left'></div>
		</td>
	</tr>
	<tr>
		<td width="10%">
		</td>
		<td align='left'>
			<div id='resumoCat' align='left'></div>
		</td>
	</tr>
	<tr>
		<td width="10%">
		</td>
		<td align='left'>
			<div id='vinculados' align='left'></div>
		</td>
	</tr>
</table>
<script>

function carregaVinculados( UF, frm){

	var div = $('vinculados');
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=vinculados&uf='+UF+'&frm='+frm,
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});
	
	
}

function carregaSubacao(){

	$('anosubacoes').style.display = '';
	
	// delcarações de variaveis
	var UF  = $('estuf').value; 
	var frm = $('frm').value;
	var ano = $('ano').value;
	var div = $('divSub');
	div.innerHTML = '';

	if( UF == '' ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select>';
		$('vinculados').innerHTML = '';
		return false;
	}

	if( frm == '' ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select>';
		$('vinculados').innerHTML = '';
		return false;
	}
	
	if( (ano == '') ){
//		&& (frm != 13 && frm != 15 ) ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select>';
		$('vinculados').innerHTML = '';
		return false;
	}
//	else if((frm == 13) || (frm == 15)){
//		$('anosubacoes').style.display = 'none';
//	}

	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=buscarSub&uf='+UF+'&frm='+frm+'&ano='+ano,
		onComplete: function(res){
			div.innerHTML = res.responseText;
			carregaVinculados(UF,frm);
		}
	});

}

function resumoSub( sub ){

	var div = $('resumoSub');
	var frm = $('frm').value;

	if( sub == ''){
		div.innerHTML = '';
		return false;
	}
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=resumoSub&sbaid='+sub+'&frm='+frm,
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});
}

function resumoCat( cat ){

	var div = $('resumoCat');

	if ( cat == ''){
		div.innerHTML = '';
		return false;
	}
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=resumoCat&catid='+cat,
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});
}

function vinculaSubCat() {		

	var uf        = $( "estuf" ).value;
	var frm       = $( "frm" ).value;
	var ano       = $( "ano" ).value;
	var subacao   = $( "subacao" ).value;
	var categoria = $( "categoria" ).value;		
	var casexibir = $( "casexibir" ).value;		

	if( uf == ''){
		alert('O campo \'UF da Subação\' é de preenchimento obrigatório.');
		return false;
	}

	if( frm == ''){
		alert('O campo \'Forma de Execução\' é de preenchimento obrigatório.');
		return false;
	}

	if( ano == ''){
		alert('O campo \'Ano\' é de preenchimento obrigatório.');
		return false;
	}
	
	if( subacao == ''){
		alert('O campo \'Subação\' é de preenchimento obrigatório.');
		return false;
	}
	
	if( categoria == ''){
		alert('O campo \'Categoria\' é de preenchimento obrigatório.');
		return false;
	}
	
	if( casexibir == ''){
		alert('O campo \'Exibição em Monitoramento\' é de preenchimento obrigatório.');
		return false;
	}

	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=vincular&categoria='+categoria+'&subacao='+subacao+'&casexibir='+casexibir,
		onComplete: function(res){
			alert(res.responseText);
			window.location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/vincula_categorias&acao=A&frm='+frm+'&uf='+uf+'&ano='+ano;
		}
	});			
}

function excluirVinculo( catid, sbaid, ano ){

	if( !confirm('Deseja realmente excluir este vinculo?') ){
        return false;
    }
	
	var uf    = $('estuf').value; 
	var frm = $('frm').value;
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=excluir&catid=' + catid + '&sbaid=' + sbaid + '&ano' + ano,
		onComplete: function(res){
			alert(res.responseText);
			carregaVinculados( uf, frm);
			carregaSubacao();
		}
	});	
	
}

function exibe( casid ){

	var div = $('exibir'+casid);
	var exibe = $(''+casid).value;
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=altexibe&casid=' + casid + '&exibe=' + exibe,
		onComplete: function(res){
			if( exibe == 0 ){
				div.innerHTML = '<a style="cursor: pointer;"><input type="hidden" id="'+casid+'" value="1"/> Ocultar</a>';
			}else{
				div.innerHTML = '<a style="cursor: pointer;"><input type="hidden" id="'+casid+'" value="0"/>Exibir</a>';
			}
		}
	});	
}

carregaSubacao();
</script>