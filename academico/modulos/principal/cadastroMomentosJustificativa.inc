<?php

function comboMomento( $dados ){
	
	global $db;
	
	$sql = "SELECT 
				slf.slfid as codigo, 
				slf.slfdsc as descricao 
			FROM 
				academico.sldfase slf
			LEFT JOIN academico.sldfasejustificativa slj ON slj.slfid = slf.slfid AND slj.sljstatus = 'A'
			WHERE
				slf.slfstatus = 'A'
				AND (slj.sljid IS NULL ".($dados['sljid'] ? "OR sljid = ".$dados['sljid'] : "" ).")";
	
	$select = $db->carregar($sql);
	$slfid = $dados['slfid'];
	
	$db->monta_combo('slfid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'slfid', '', $dados['slfid']);
}

function carregar($dados){
	
	global $db;
	pg_set_client_encoding('UTF-8');
	$sql = "SELECT
				sljid,
				slfid,
				sljdsc
			FROM
				academico.sldfasejustificativa
			WHERE
				sljid = ".$dados['sljid'];
	
	$retorno = $db->pegaLinha($sql);
	
	echo simec_json_encode($retorno);
}

function salvar($dados){
	
	global $db;
	
	if($dados['sljid']!=''){

		$sql = "UPDATE academico.sldfasejustificativa SET
					slfid = ".$dados['slfid'].",
					sljdsc = '".utf8_decode($dados['sljdsc'])."',
					sljdataatualizacao = now()
				WHERE
					sljid = ".$dados['sljid'];	
	}else{
		
		$sql = "INSERT 
					INTO academico.sldfasejustificativa(slfid, sljdsc, sljdataatualizacao) 
					VALUES (".$dados['slfid'].",'".utf8_decode($dados['sljdsc'])."', now())";
	}
	
	if($db->executar($sql)){
		$db->commit();
		echo listaJustificativas();
	}else{
		$db->rollback();
	}
}

function excluir($dados){
	
	global $db;

	$sql = "UPDATE academico.sldfasejustificativa SET
				sljstatus = 'I'
			WHERE
				sljid = ".$dados['sljid'];	
	
	if($db->executar($sql)){
		$db->commit();
		echo listaJustificativas();
	}else{
		$db->rollback();
	}
}

function listaJustificativas(){
	
	global $db;
	
	$perfilNotBloq = array(PERFIL_ADMINISTRADOR);
	// Verificando a segurança
	$permissoes = verificaPerfilAcademico($perfilNotBloq);
	
	$sql = "SELECT".( $permissoes ? 
				"'<img class=\"alterar\" id=\"'|| sljid ||'\" src=\"../imagens/alterar.gif\" style=\"cursor: pointer\" border=\"0\" align=\"absmiddle\" title=\"Alterar\" />
				 <img class=\"excluir\" id=\"'|| sljid ||'\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" align=\"absmiddle\" title=\"Excluir.\" />',":"")."
				slfdsc, 
				sljdsc, 
				to_char(sljdataatualizacao, 'DD/MM/YYYY')
			FROM 
				academico.sldfasejustificativa slj
			INNER JOIN academico.sldfase slf ON slf.slfid = slj.slfid 
			WHERE
				sljstatus = 'A' ";
	
	if($permissoes){
		$cabecalho = array("Ações", "Momento", "Justificativa", "Ultima atualização");
	}else{
		$cabecalho = array("Momento", "Justificativa", "Ultima atualização");
	}
	return $db->monta_lista($sql, $cabecalho, 50, 20, '', '100%', '',$arrayDeTiposParaOrdenacao);
}

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$_SESSION['academico']['entidadenivel'] = 'unidade';

if($_REQUEST['req']) {
	$_REQUEST['req']($_REQUEST);
	exit;
}

if(!$_SESSION['academico']['entid']) {
	
	die("<script>
			alert('Entidade não encontrada. Navegue novamente.');
			window.location='academico.php?modulo=inicio&acao=C';
		 </script>");
	
}

$perfilNotBloq = array(PERFIL_ADMINISTRADOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

$abacod_tela = $_SESSION['academico']['orgid'] == '2' ? '57334' : '57335';

#$db->cria_aba($abacod_tela,$url,$parametros);
#academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Momento - Cadastro de Justificativas');


?>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script>

jQuery.noConflict();

jQuery(document).ready(function() {

	jQuery('#salvar').click(function(){

		jQuery(this).attr('disabled',true);

		var pendencia = false;
		
		jQuery('.obrigatorio').each(function(){
			var val = jQuery.trim(jQuery(this).val());
			if( val.length == 0 ){
				jQuery(this).focus();
				pendencia = true;
				return false;
			}
		});

		if(pendencia){
			alert('Campo obrigatório.');
			jQuery(this).attr('disabled',false);
			return false;
		}

		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "&req=salvar&"+jQuery('#formulario').serialize(),
			async: false,
			success: function(msg){
				if(msg!=''){
					jQuery('#lista').html(msg);
				}
			}
		});
		jQuery(this).attr('disabled',false);

		alert('Operação realizada com sucesso.');
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "&req=comboMomento",
			async: false,
			success: function(msg){
				jQuery('#momento').html(msg);
			}
		});
		jQuery('#sljid').val(null);
		jQuery('#sljdsc').val(null);
	});

	jQuery('.alterar').live('click',function(){
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "&req=carregar&sljid="+jQuery(this).attr('id'),
			async: false,
			dataType: 'json',
			success: function(msg){
				jQuery.ajax({
					type: "POST",
					url: window.location,
					data: "&req=comboMomento&sljid="+msg.sljid+"&slfid="+msg.slfid,
					async: false,
					success: function(msg){
						jQuery('#momento').html(msg);
					}
				});
				jQuery('#sljid').val(msg.sljid);
				jQuery('#sljdsc').val(msg.sljdsc);
			}
		});
	});

	jQuery('.excluir').live('click',function(){
		if(confirm('Deseja excluir esta justificativa?')){
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "&req=excluir&sljid="+jQuery(this).attr('id'),
				async: false,
				success: function(msg){
					jQuery.ajax({
						type: "POST",
						url: window.location,
						data: "&req=comboMomento",
						async: false,
						success: function(msg){
							jQuery('#momento').html(msg);
						}
					});
					jQuery('#sljid').val(null);
					jQuery('#sljdsc').val(null);
					jQuery('#lista').html(msg);
				}
			});
		}
	});
});

</script>
<?monta_titulo('Momento - Cadastro de Justificativas', $linha2);?>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    // cria o cabeçalho padrão do módulo
    $autoriazacaoconcursos = new autoriazacaoconcursos();
    echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);

if($permissoes){?>
<form id="formulario" name="formulario" method="post" action="academico.php?modulo=principal/solicitacaodecreto&acao=A">
	<input type="hidden" id="sljid" name="sljid" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
		<tr>
			<td class="SubTituloDireita">Momento:</td>
			<td id="momento">
				<?
				$sql = "SELECT 
							slf.slfid as codigo, 
							slf.slfdsc as descricao 
						FROM 
							academico.sldfase slf
						LEFT JOIN academico.sldfasejustificativa slj ON slj.slfid = slf.slfid AND slj.sljstatus = 'A'
						WHERE
							slf.slfstatus = 'A'
							AND slj.sljid IS NULL";
				$db->monta_combo('slfid', $sql, 'S', 'Selecione', '', '', '', '200', 'S', 'slfid');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Justificativa:</td>
			<td>
				<?
				echo campo_textarea('sljdsc', 'S', 'S', '', '130', '20', '');
				?>
			</td>
		</tr>
		<tr>
			<td align="center" colspan="2">
				<input type="button" id="salvar" value="Salvar" > 
			</td>
		</tr>
	</table>
</form>
<?php }?>
<div id="lista">
	<?=listaJustificativas() ?>
</div>
</div>