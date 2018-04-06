<?php 

require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";

# Verifica se a sessão não expirou, se tiver expirada envia pra tela de login.
controlarAcessoSemAutenticacao();

$db = new cls_banco();

if($_REQUEST['requisicao'] == 'buscarMunicipios'){
	
	$stWhere = '';
	if($_REQUEST['muncod']){
		$stWhere .= " and muncod ".($_REQUEST['remover'] == 'true' ? ' in ' : ' not in ')." ('".implode("','",$_REQUEST['muncod'])."') ";
	}	
	if(trim($_REQUEST['q'])){
		$stWhere .= " and (mundescricao ilike '%{$_REQUEST['q']}%' or muncod = '{$_REQUEST['q']}') ";
	}
	
	$sql = "select 
				muncod as codigo, 
				muncod || ' - ' || mundescricao || '/' || estuf as descricao 
			from 
				territorios.municipio
			where 
				muncod is not null  
			 ".$stWhere."  
			order by 
				mundescricao";
	
	$rs = $db->carregar($sql);
		
	$rs = $rs ? $rs : array();	
	foreach($rs as $dados){
		echo $dados['descricao']."|".$dados['codigo']."\n";
	}	
	die;
}

ini_set("memory_limit","1024M");

if($_POST) extract($_POST); 

if($mundescricao){
	$arWhere[] = " mun.mundescricao ilike '%{$mundescricao}%' ";
}

$sql = "select 
			case when est.estuf is null then mun.estuf else est.estuf end as estuf,
			mun.muncod, 
			est.estdescricao,
			mun.mundescricao 
		from 
			territorios.estado est
		inner join 
			territorios.municipio mun on mun.estuf = est.estuf
		".(is_array($arWhere) ? ' WHERE '.implode(' AND ', $arWhere) : '')."  
		order by 
			mun.estuf, est.estdescricao, mun.mundescricao";

$rs = $db->carregar($sql);

$arUfs = array();
?>
<script src="../includes/JQuery/jquery-1.4.2.js" type="text/javascript"></script>
<script src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js" type="text/javascript"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script>

	$(function(){

		var nome_campo = $('[name=nome_temp]').val();
		var combo = window.opener.document.getElementById(nome_campo);

		// Preenche checks dos municipios	
		if(combo.length > 0){
			for(x=0;x<combo.length;x++){
				if(combo[x].value && document.getElementById("muncod_"+combo[x].value)){
					document.getElementById("muncod_"+combo[x].value).checked = true;	
				}
			}	
		}
		
		$('.mostraMunicipios').click(function(){
			$('.tr_'+this.id+', #img_mais_'+this.id+', #img_menos_'+this.id).toggle();
		});
		
		$('#btnPesquisar').click(function(){
			$('#formPesquisa').submit();
		});

		$('#btnLimpar').click(function(){
			document.location.href = 'popup_municipios.php?nome='+nome_campo;
		});

		$('#btnFechar').click(function(){
			window.close();
		});
		
	});

	function marcarMunicipio( muncod, obj ){
		
		var nomeCampo = $('[name=nome_temp]').val();
		var janela = window.opener.document;
		var campo = janela.getElementById(nomeCampo);
		
		if(obj.checked){	
			window.opener.combo_popup_adicionar_item(nomeCampo , muncod, obj.title, true );
		}else{
			window.opener.combo_popup_remover_item( nomeCampo, muncod, true );
		}
	}
	
</script>

<form name="formPesquisa" id="formPesquisa" method="post">
	<input type="hidden" name="nome_temp" value="<?php echo $_REQUEST['nome']?>" />
	<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">		
		<tr>
			<td class="subtituloDireita">Município:</td>
			<td><?php echo campo_texto('mundescricao', 'N', 'S', '', 40, 255, '', ''); ?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">&nbsp;</td>
			<td class="subtituloEsquerda">
				<input type="button" value="Pesquisar" id="btnPesquisar" />
				<input type="button" value="Limpar" id="btnLimpar" />
				<input type="button" value="Fechar" id="btnFechar" />
			</td>
		</tr>
	</table>
</form>

<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
	<?php 
	$x=1;
	$y=1;
	?>
	<?php foreach($rs as $dados): ?>
		<?php if(!in_array($dados['estuf'], $arUfs)): ?>
			<?php $corX = $x % 2 ? '#F7F7F7' : 'white'; $x++; ?>
			<?php $arUfs[] = $dados['estuf']; ?>
			<tr bgcolor="<?php echo $corX; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $corX; ?>';">
				<td class="mostraMunicipios" id="<?php echo $dados['estuf'] ?>">
					<img src="../imagens/mais.gif" id="img_mais_<?php echo $dados['estuf']; ?>" />
					<img src="../imagens/menos.gif" id="img_menos_<?php echo $dados['estuf']; ?>" style="display:none;" />
					&nbsp;<a href="javascript:void(0);"><?php echo $dados['estuf']; ?></a>
				</td>
			</tr>			
		<?php endif; ?>
		<?php $corY = $y % 2 ? '#F7F7F7' : 'white'; $y++; ?>
		<tr class="tr_<?php echo $dados['estuf']; ?>" style="display:none;" bgcolor="<?php echo $corY; ?>" onmouseover="this.bgColor='#ffffcc';" onmouseout="this.bgColor='<?php echo $corY; ?>';">
			<td>					
				<input type="checkbox" name="muncod[]" id="muncod_<?php echo $dados['muncod'] ?>" value="<?php echo $dados['muncod']; ?>" onclick="javascript:marcarMunicipio('<?php echo $dados['muncod']; ?>', this)" title="<?php echo str_replace("'",'',$dados['mundescricao']); ?>" />
				<?php echo str_replace("'",'',$dados['mundescricao']); ?>
			</td>
		</tr>				
	<?php endforeach; ?>
</table>