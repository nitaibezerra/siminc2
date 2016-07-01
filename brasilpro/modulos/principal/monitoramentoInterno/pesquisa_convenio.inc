<?php

function buscarConvenios( $dado = null ){
	
	global $db;

	$where = "WHERE 1=1";
	if($dado['cmosigla']){
		$where .= "AND cmosigla LIKE '{$dado['cmosigla']}'";
	}
	if($dado['cmodsc']){
		$where .= "AND cmodsc ILIKE '%{$dado['cmodsc']}%'";
	}
	if($dado['cmoano']){
		$where .= "AND cmoano = {$dado['cmoano']}";
	}
	if($dado['uf']){
		$where .= "AND estuf ILIKE '%{$dado['uf']}%'";
	}
	$sql = "SELECT
				'<img onclick=\"return alterarConvenio('|| cmoid ||')\" src=\"/imagens/alterar.gif\" border=\"0\" title=\"Alterar Registro\" />
				 <img onclick=\"return excluirConvenio('|| cmoid ||')\" src=\"/imagens/excluir.gif\" border=\"0\" title=\"Excluir Registro\" />' 
				 as acoes, 
				cmodsc || '/' || cmoano || ' - ' || cmosigla as descricao,
				estuf as estado
			FROM
				cte.conveniomonitoramento
				{$where} 
			ORDER BY
				estado,
				descricao";
	
	$db->monta_lista( $sql, array('Ações','Convênio','UF'), 50, 1000, 'N','','','convenios',array('10%','70%','20%'),array('center','left') );
	
}

if( $_POST['req'] == buscar ){
	
	buscarConvenios($_POST);
	die();	
}

if( $_POST['req'] == 'excluir' ){
	
	global $db;
	
	$vinculos = $db->pegaUm("SELECT
								count(*)
							 FROM 
							 	cte.conveniosubacao
							 WHERE
							 	cmoid = {$_POST['cmoid']}");
	if( $vinculos == 0 ){
		$sql = "DELETE FROM cte.conveniomonitoramento
				WHERE cmoid = ".$_POST['cmoid'];
		
		$db->executar($sql);
		$db->commit();
		echo "true";
		die();
	}
	echo "false";
	die();
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
		   			array ("descricao" => "Vincular Convênios nas Subações",
							    "id"		=> "3",
							    "link"		=> "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/vincula_convenio&acao=A"
					  		   )
			  );
	}
	
echo montarAbasArray($res, "/brasilpro/brasilpro.php?modulo=principal/monitoramentoInterno/pesquisa_convenio&acao=A");

?>

<script type="text/javascript" src="../includes/funcoes.js"></script>
<script src="../includes/prototype.js"></script>
<script src="../includes/entidades.js"></script>
<script src="../includes/calendario.js"></script>

<form action="<?php echo $_SERVER['REQUEST_URI'] ?>" method="post" id="formulario" name="formulario">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1" >
	<tr>
		<td width="100%" align="center" gbcolor="#dcdcdc" colspan="2" bgcolor="#dcdcdc" >
			<label class="TituloTela" style="color: rgb(0, 0, 0);">Pesquisa de Convênios</label>
		</td>
	</tr>
	<tr>
		<td style="" align="center" bgcolor="#e9e9e9" colspan="2">
			&nbsp;
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width='10%'>
			Sigla:
		</td>
		<td width='50%' valign="center">
			<?php 
				$sql = "SELECT DISTINCT
							cmosigla as codigo,
							cmosigla || '' as descricao
						FROM
							cte.conveniomonitoramento
						ORDER BY
							cmosigla";
				
				$db->monta_combo( 'cmosigla', $sql, 'S', 'Selecione', '', '' ,'','100','N','cmosigla', '', $_REQUEST["cmosigla"] );
			?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita' width='10%'>
			Código / Ano:
		</td>
		<td width='50%' valign="center">
			<?php 
			$_REQUEST['cmoano'] = $_REQUEST['cmoano'] ? $_REQUEST['cmoano'] : ''; 
			$_REQUEST['cmodsc'] = $_REQUEST['cmodsc'] ? $_REQUEST['cmodsc'] : ''; 
			?>
			<?= campo_texto( 'cmodsc', 'N', 'S', '', 26, 26, '####################', '','','','','id="cmodsc"','','','this.value=mascaraglobal(\'####################\',this.value);' ); ?>
			&nbsp; / &nbsp;
			<select id="cmoano" name="cmoano" onchange="" >
				<option value="<?=$_REQUEST['ano']?>"><?=$_REQUEST['ano'] ?></option>
				<option value=""></option>
				<?php for($x=2008;$x<=date('Y');$x++): ?>
					<option value="<?php echo $x ?>"><?php echo $x ?></option>
				<?php endfor; ?>
			</select>
		</td>
		<td></td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
			UF:
		</td>
		<td>
			<?php 
			$sql = "SELECT
						estuf AS codigo,
						estuf || ' - ' || estdescricao AS descricao
					FROM
						territorios.estado
					ORDER BY
						estuf";
	
	 		echo $db->monta_combo('estuf',$sql,'S',"-- Selecione para filtrar --",'','','',170,'','estuf');
			?>
		</td>
	</tr>
	<tr>
		<td class='SubTituloDireita'>
		</td>
		<td>
			<input type="button" id="btnBuscar" name="btnBuscar" value="Buscar" onclick="buscarConvenios()"/>
		</td>
	</tr>
	<tr align='left'>
		<td>
		</td>
		<td align='left'>
			<div id='listaConvenios' align='left'>
				<?=buscarConvenios();?>
			</div>
		</td>
	</tr>
</table>
</form>
<script>
function buscarConvenios() {		

	var cmosigla = document.getElementById( "cmosigla" ).value;
	var cmodsc   = document.getElementById( "cmodsc" ).value;
	var cmoano   = document.getElementById( "cmoano" ).value;
	var uf       = document.getElementById( "estuf" ).value;
	var div      = document.getElementById( "listaConvenios" );		

	var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: '&req=buscar&cmodsc=' + cmodsc + '&uf=' + uf + '&cmoano=' + cmoano + '&cmosigla=' + cmosigla,
				asynchronous: false,
				onComplete: function(resp){
					div.innerHTML = resp.responseText;
				},
				onLoading: function(){
					div.innerHTML = "carregando...";
				}
		});		
}

function excluirConvenio( cmoid ) {

	if( !confirm('Deseja realmente excluir esta categoria?') ){
        return false;
    }
	
	return new Ajax.Request(window.location.href,{
		method: 'post',
		parameters: '&req=excluir&cmoid=' + cmoid,
		onComplete: function(res){
			if( res.responseText == 'true' ){
				alert('Convenio Excluido.');
			}else{
				alert('Convenio Excluido.');
			}
			buscarConvenios();
		}
	});	
}

function alterarConvenio( cmoid ) {

	window.location.href = 'brasilpro.php?modulo=principal/monitoramentoInterno/cadastra_convenio&acao=A&req=importar&cmoid='+cmoid;
	
}



</script>