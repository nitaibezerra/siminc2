<?php

if( $_POST['req'] == 'excluir' ){
	
	$testa = 0;
	
	$sql = "SELECT 
				count(csuid)
			FROM 
				cte.monitoramentosubacoes
			WHERE 
				sbaid = ".$_POST['sbaid'];
	
	$testa = $db->pegaUm($sql);
	
	if( $testa == 0 ){
		$sql = "DELETE FROM 
					cte.conveniosubacao
				WHERE cmoid = ".$_POST['cmoid']." AND
					  sbaid = ".$_POST['sbaid'];
		echo "Vínculo excluído.";
	}else{
		echo "Não é possível excluir este vínculo, pois existe monitoramento em andamento para esta subação.";
	}
	
	$db->executar($sql);
	$db->commit();
	die();
}

function buscarConv( $uf = null ){
	
	global $db;
	
	$sql = "SELECT 
				cmoid   as codigo, 
				cmodsc || '/' || cmoano || ' - ' || cmosigla as descricao 
			FROM 
				cte.conveniomonitoramento 
			WHERE
				estuf = '{$uf}'
			ORDER BY 
				cmodsc, 
				cmoano asc";

	echo $db->monta_combo( 'convenio', $sql, 'S', 'Selecione', '', '','','200','S','convenio' );
	
}

if( $_POST['req'] == 'buscarConv' ){
	
	buscarConv( $_REQUEST['uf'] );
	
	die();
}

if( $_POST['req'] == 'buscarSub' ){
	
	if( $_POST['uf'] == '' ){
		echo "<select disabled >
				<option value=''>Escolha um estado.</option>
			  </select>";
		die();
	}
	
	// DECLARAÇÕES DE VARIAVEIS
	$complementoFrom = " ";
	
	// MONTA SQL DE ACORDO COM A ESCOLHA DO USUÁRIOS PELOS COMBOS DE FORMA DE EXECUÇÃO
	if($_POST['frm'] == '17' ){ // forma de execução assistencia financeira
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
				INNER JOIN cte.categoriasubacao      cs  ON cs.sbaid    = s.sbaid
				".$complementoFrom."
			WHERE iu.estuf = '".$_POST['uf']."' 
			AND s.frmid = ".$frmid."
			AND iu.itrid = 3
			AND s.sbaidpai is null
			ORDER BY descricao ";
	$db->monta_combo( 'subacao', $sql, 'S', 'Selecione', 'monitorados(this.value);resumoSub', '','','200','S','subacao' );
	
	die();
}

if( $_POST['req'] == 'monitorado' ){
	
	$sql = "SELECT DISTINCT
				sbaid
			FROM
				cte.monitoramentosubacoes
			WHERE
				sbaid = {$_POST['sbaid']} AND
				csuid is null";
	
	$sbaid = $db->pegaUm($sql);
	
	echo $sbaid;
	
	die();
	
}

if( $_POST['req'] == 'vinculados' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
	global $db;
	
	$sql = "SELECT 
				'<img onclick=\"return excluirVinculo('|| cm.cmoid ||', '|| s.sbaid ||', '|| CASE WHEN ps.pssano IS NULL THEN 0 ELSE ps.pssano END ||' )\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' as acoes,
				(dim.dimcod || '.' || 
				 are.ardcod || '.' || 
				 ind.indcod || ' ' ||	
				 SUBSTRING(s.sbadsc, 1, 150)) as subacao,
				cm.cmodsc || '/' || cm.cmoano || ' - ' || cmosigla as convenio
			FROM cte.subacaoindicador s
				INNER JOIN cte.acaoindicador         a   ON s.aciid     = a.aciid
				INNER JOIN cte.pontuacao             p   ON p.ptostatus = 'A' AND p.ptoid = a.ptoid
				INNER JOIN cte.instrumentounidade    iu  ON iu.inuid    = p.inuid 
				INNER JOIN cte.indicador             ind ON ind.indid   = p.indid
				INNER JOIN cte.areadimensao          are ON are.ardid   = ind.ardid
				INNER JOIN cte.dimensao              dim ON dim.dimid   = are.dimid
				INNER JOIN cte.conveniosubacao       cs  ON cs.sbaid    = s.sbaid
				INNER JOIN cte.conveniomonitoramento cm  ON cm.cmoid    = cs.cmoid
				LEFT JOIN cte.projetosapesubacao     ps  ON ps.sbaid    = s.sbaid
			WHERE iu.estuf = '".$_POST['uf']."' AND s.frmid =".$_POST['frm']." AND iu.itrid = 3
			GROUP BY subacao, convenio, cm.cmoid, s.sbaid,ps.pssano";
	
	echo "
		<table align=\"center\" border=\"0\" class=\"tabela\" cellpadding=\"3\" cellspacing=\"1\">
			<tr>
				<td align=\"center\" gbcolor=\"#dcdcdc\" colspan=\"2\" bgcolor=\"#dcdcdc\" >
					<label class=\"SubTitulo\" style=\"color: rgb(0, 0, 0);\">Subações Vinculadas</label>
				</td>
			</tr>
			<tr>
				<td>";
	
	$db->monta_lista( $sql, array('Ações','Subação','Convênio'), 99999999, 99999999, 'N','','','vinculacoes',array('5%','55%','40%'),array('center','left','left') );
			
	echo "</td></tr></table>";
	
	die();
}

if( $_POST['req'] == 'resumoSub' ){
	
	header('Content-type: text/html;charset=ISO-8859-1');
	
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


if( $_POST['req'] == 'vincular' ){
	
	$teste = $db->pegaUm("SELECT
							count(sbaid)
						  FROM
							cte.conveniosubacao
						  WHERE
							cmoid = {$_POST['cmoid']} AND
							sbaid = {$_POST['subacao']}");
	
	if( $teste > 0 ){
		
		echo "Este vinculo já existe.";
		
		die();
		
	}else{
		$sql = "INSERT INTO 
					cte.conveniosubacao(cmoid,sbaid)
				VALUES 
					({$_POST['cmoid']},{$_POST['subacao']}) 
				RETURNING 
					csuid";
		
		$csuid = $db->pegaUm($sql);
		
		if( $_POST['sbaid'] != 0 ){
			
			$sql = "UPDATE cte.monitoramentosubacoes
					SET 
						csuid = {$csuid}
					WHERE 
						sbaid = {$_POST['sbaid']}";
			
			$db->executar($sql);
			
		}
		
		$db->commit();
		
		echo "Subação vinculada com sucesso.";
		
		die();
	}
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';
print '<br/>';

$res = array( 0 =>
					array ("descricao" => "Pesquisa de Convênios",
							    "id"        => "1",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_convenio&acao=A"
							   )
			);

if( $db->testa_superuser() ){	
		array_push($res,
					array ("descricao" => "Cadastrar Convênios",
							    "id"        => "2",
							    "link" 		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A"
							   ),
		   			array ("descricao" => "Vincular Convenios nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_convenio&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_convenio&acao=A");
?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="2" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Vincular convênios nas subações</label>
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
				
	            $sql = "SELECT 
	            			e.estuf as codigo, 
	            			e.estdescricao as descricao 
	            		FROM 
	            			territorios.estado e 
	            		WHERE 
	            			e.estuf in ({$unidades}) 
	            		ORDER BY 
	            			e.estdescricao asc";
	            
	            $db->monta_combo( 'estuf', $sql, 'S', 'Selecione', 'carregaConv();carregaSubacao', '' ,'','200','S','estuf', '', $_REQUEST["uf"] );
            ?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Forma de Execução:
		</td>
		<td>
			<?php 
			
				$sql = "SELECT 
							frmid as codigo, 
							frmdsc as descricao 
						FROM 
							cte.formaexecucao 
						WHERE 
							frmbrasilpro = true AND 
							frmtipo = 'E' AND 
							frmid not in (19 ,21)
						ORDER BY 
							frmdsc";
				
	            $db->monta_combo( 'frm', $sql, 'S', 'Selecione', 'carregaSubacao', '','','200','S','frm','',$_REQUEST['frm'] );
            ?>
		</td>
	</tr>
	<tr id="anosubacoes" >
		<td class='SubTituloDireita' width="10%">
			Ano da Subação:
		</td>
		<td>
			<?php 
			$_REQUEST['ano'] = $_REQUEST['ano'] ? $_REQUEST['ano'] : ''; 
			?>
			<select id="ano" name="ano" onchange="carregaSubacao()" >
				<option value="<?=$_REQUEST['ano']?>"><?=$_REQUEST['ano'] ?></option>
				<option value=""></option>
				<?php for($x=2008;$x<=date('Y');$x++): ?>
					<option value="<?php echo $x ?>"><?php echo $x ?></option>
				<?php endfor; ?>
			</select>
			<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
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
						<option value="">....</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php 					
					}
				 ?>
			</div>
			<input type="hidden" id="monitorado" name="monitorado" value="">
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
			Convênio:
		</td>
		<td>
			<div id='divConv'>
				<?php 
					if( $_REQUEST['uf'] == '' ){
				?>
					<select disabled id="subacao" name="subacao">
						<option value="">Escolha um estado.</option>
					</select>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				<?php 					
					}else{
						echo buscarConv( $_REQUEST['uf'] );
					}
				 ?>
			</div>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width="10%">
		</td>
		<td>
			<input type="button" id="btnVincular" name="btnVincular" value="Vincular" onclick="vinculaSubConv()"/>
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
			<div id='vinculados' align='left'></div>
		</td>
	</tr>
</table>
<script>

function carregaVinculados( UF, frm ){

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
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
		$('vinculados').innerHTML = '';
		return false;
	}

	if( frm == '' ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
		$('vinculados').innerHTML = '';
		return false;
	}
	
	if( (ano == '') ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value="">....</option></select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
		$('vinculados').innerHTML = '';
		return false;
	}

	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=buscarSub&uf='+UF+'&frm='+frm+'&ano='+ano,
		onComplete: function(res){
			div.innerHTML = res.responseText;
			carregaVinculados(UF,frm);
		}
	});

}

function carregaConv(){

	// delcarações de variaveis
	var UF  = $('estuf').value; 
	var div = $('divConv');
	div.innerHTML = '';

	if( UF == '' ){
		$('resumoSub').innerHTML = '';
		div.innerHTML = '<select disabled id="subacao" name="subacao"><option value=""> Escolha um estado. </option></select><img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">';
		$('vinculados').innerHTML = '';
		return false;
	}

	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=buscarConv&uf='+UF,
		onComplete: function(res){
			div.innerHTML = res.responseText;
		}
	});

}

function monitorados( sbaid ){

	// delcarações de variaveis
	var subacao		= $( "subacao" ).value;
	var monitorado  = $('monitorado'); 

	if( subacao == '' ){
		return false;
	}

	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=monitorado&sbaid='+subacao,
		onComplete: function(res){
			monitorado.value = res.responseText;
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

function vinculaSubConv() {		

	var uf         = $( "estuf" ).value;
	var frm        = $( "frm" ).value;
	var ano        = $( "ano" ).value;
	var subacao    = $( "subacao" ).value;
	var convenio   = $( "convenio" ).value;		
	var monitorado = $( "monitorado" ).value;		

	if( uf == ''){
		alert('O campo \'UF da Subação\' é de preenchimento obrigatório.');
		return false;
	}

	if( frm == ''){
		alert('O campo \'Forma de Execução\' é de preenchimento obrgatório.');
		return false;
	}

	if( ano == ''){
		alert('O campo \'Ano\' é de preenchimento obrgatório.');
		return false;
	}
	
	if( subacao == ''){
		alert('O campo \'Subação\' é de preenchimento obrgatório.');
		return false;
	}
	
	if( convenio == ''){
		alert('O campo \'Convênio\' é de preenchimento obrgatório.');
		return false;
	}

	var sbaid = 0;
		
	if( monitorado != '' ){
		if( confirm('Essa subação já está sendo monitorada. Deseja que o monitoramento que está sendo realizado seja migrado para esse convênio?') ){
    		sbaid = monitorado;
		}
	}
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=vincular&cmoid=' + convenio + '&subacao='+ subacao+'&sbaid='+sbaid,
		onComplete: function(res){
			alert(res.responseText);
			window.location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/vincula_convenio&acao=A&frm='+frm+'&uf='+uf+'&ano='+ano;
		}
	});			
}

function excluirVinculo( cmoid, sbaid, ano ){

	if( !confirm('Deseja realmente excluir este vinculo?') ){
        return false;
    }
	
	var uf  = $('estuf').value; 
	var frm = $('frm').value;
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=excluir&cmoid=' + cmoid + '&sbaid=' + sbaid + '&ano' + ano,
		onComplete: function(res){
			alert(res.responseText);
			carregaVinculados( uf, frm);
			carregaSubacao();
		}
	});	
	
}

carregaSubacao();
</script>