<?php

include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";
$db->cria_aba( $abacod_tela, $url, $parametros );
monta_titulo( 'Enviar Emendas Impositivo para o SIOP', '' );

?>
<script type="text/javascript" src="../estrutura/js/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.14.custom.min.js"></script>
<link rel="stylesheet" href="../includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" type="text/css" media="all" />
<style>
.button{
	width: 120px; 
	height: 25px;
}
</style>


<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" name="wsusuario" id="wsusuario" value="">
	<input type="hidden" name="wssenha" id="wssenha" value="">
	<input type="hidden" name="wsperfil" id="wsperfil" value="">
	<input type="hidden" name="wsmomento" id="wsmomento" value="">
	<input type="hidden" name="wssolicitacao" id="wssolicitacao" value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Origem Emenda:</td>
			<td>
			<?php 
				
				$etoid = $_REQUEST["etoid"];
				
				$sql = "select 
							o.etoid as codigo,
							o.etodescricao as descricao
						 from emenda.emendatipoorigem o
						 where o.etostatus = 'A'
						 order by o.etodescricao";
				
				$db->monta_combo("etoid",$sql,'S',"Todas", 'pesquisarEmenda','','','300','N','etoid');
			?>
			</td>
		</tr>
	</table>

	<table class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
		<tr>
			<td class="subtituloesquerda"><input type="button" name="btnEnviarSiop" id="btnEnviarSiop" value="Enviar SIOP" onclick="enviarSiop();">&nbsp;
					<input type="button" name="btnVisualizar" id="btnVisualizar" value="Visualizar Log SIOP" onclick="visualizarLog();"></td>
		</tr>
		<tr>
			<td>
	<?
	$where = '';
	if( $_POST['requisicao'] == 'pesquisar' ){
		if( $_POST['etoid'] ) $where = ' and e.etoid = '.$_POST['etoid'];
	}
	
	if( $_SESSION['exercicio'] > 2014 ){
		$filtroImp = ' and ei.edeid = ede.edeid';
		$joinImp = " inner join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A' ";
	}
	
	$sql = "SELECT
					'<center><input type=\"checkbox\" class=\"marcar_emenda\" name=\"chk[]\" id=\"chk\" value=\"'||ed.emdid||'\"></center>' as acoes,
					e.emecod,
					vfun.unicod||' - '||un.unidsc as unidade,
					case when e.emerelator = 'S' then a.autcod||' - '||a.autnome||' - Relator Geral' else a.autcod||' - '||a.autnome end as autor,
					vfun.fupfuncionalprogramatica||'&nbsp;' as funcional,
					--vfun.fupdsc,
					ed.gndcod||' - '||gn.gnddsc as gndcod1, 
		            ed.mapcod||' - '||map.mapdsc as modalidade,
		            ed.foncod||' - '||fon.fondsc as fonte,
					sum(ed.emdvalor) as valor,
					ei.edivalor,
					case when ei.ediimpositivo = 'NH' then 'Não Há'
				    	when ei.ediimpositivo = 'TO' then 'Total'
				        when ei.ediimpositivo = 'PA' then 'Parcial'
				    else '-' end as impedimento,
					to_char(ed.emddtultimoenviosiop, 'DD/MM/YYYY HH24:MI:SS') as data
				FROM
					emenda.emenda e
					inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
					$joinImp
					inner join emenda.autor a on a.autid = e.autid
					inner join emenda.v_funcionalprogramatica vfun on vfun.acaid = e.acaid and vfun.prgano = '{$_SESSION['exercicio']}' and vfun.acastatus = 'A'
					left join public.gnd gn on gn.gndcod = ed.gndcod and gn.gndstatus = 'A'
		            left join public.modalidadeaplicacao map on map.mapcod = ed.mapcod
		            left join public.fonterecurso fon on fon.foncod = ed.foncod and fon.fonstatus = 'A'
		            inner join emenda.emendadetalheimpositivo ei on ei.emdid = ed.emdid $filtroImp
		            inner join public.unidade un on un.unicod = vfun.unicod
				WHERE
					e.emeano = '{$_SESSION['exercicio']}'
					and e.emetipo = 'E'
					and ed.emdimpositiva = 6 
					and a.autnome not ilike '%bancada%'
					--and vfun.unicod not in ('26101', '26298')
					$where
				GROUP BY
		          e.emecod, a.autnome, e.emerelator, vfun.fupfuncionalprogramatica, ei.ediimpositivo,
		          ed.emdid, ed.foncod, ed.gndcod, vfun.unicod, un.unidsc, a.autcod,
		          ed.mapcod, gn.gnddsc, map.mapdsc, fon.fondsc, ei.edivalor, e.emeid, ed.emddtultimoenviosiop
		        order by e.emecod";
	
	$cabecalho = array('Ação<br><input type="checkbox" class="marcar-todos" marcar="marcar_emenda">', 'Código', 'UO', 'Autor', 'Funcional Programática', 'GND', 'Mod', 'Fonte', 'Valor', 'Valor Impedimento', 'Impedimento', 'Ultimo Envio Ao Siop');
	$db->monta_lista_simples($sql, $cabecalho, 60000000, 1, '', '100%', '', '', '', '', true);
	?>
		</td>
	</tr>
	</table>
</form>
<?php
$usersiop = WEB_SERVICE_SIOP_USUARIO;
$passsiop = WEB_SERVICE_SIOP_SENHA;
?>
<div id="dialog_recurso" title="Enviar Emendas Impositivo para o SIOP" style="display: none" >
	<table class="tabela" style="width: 100%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center" border="0">
	<tr>
		<td style="font-size: 14px;">
			<div id="div_user" style="display: none">
				<b>Usuário SIOP:</b>
				<input type="text" class="normal" name="usersiop" id="usersiop" value="<?=$usersiop; ?>" readonly="readonly" size="30" maxlength="15"><br>
				<b>Senha SIOP:<br></b>
				<input type="password" class="normal" name="passsiop" id="passsiop" value="<?=$passsiop; ?>" readonly="readonly" size="30" maxlength="15">
			</div>
			<b>Perfil:</b>
			<input type="text" class="normal" name="userperfil" id="userperfil" value="31" size="30" maxlength="15"><br>
			<b>Momento:<br></b>
			<?
			$sql = "#1000 – Unidade Orçamentária; 2000 – Órgão Central; 2500 – SRI; 3000 – SOF; 5000 – Formalização";
			$arrMomento = array(
							array('codigo' => '1000', 'descricao' => '1000 - Unidade Orçamentária'),
							array('codigo' => '2000', 'descricao' => '2000 - Órgão Central'),
							array('codigo' => '2500', 'descricao' => '2500 - SRI'),
							array('codigo' => '3000', 'descricao' => '3000 - SOF'),
							array('codigo' => '5000', 'descricao' => '5000 - Formalização'),
							);
			$db->monta_combo("usermomento", $arrMomento, "S", "", '', '', '', '300', 'N','usermomento');
			?><br>
			</td>
	</tr>
	<tr>
		<td style="font-size: 14px;">
			<b>Selecione o tipo de solicitação:</b><br>
			&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="rad_solicitacao" id="rad_solicitacao" value="obterAnalisesEmendas">Obter Análises Emendas<br>
			&nbsp;&nbsp;&nbsp;&nbsp;<input type="radio" name="rad_solicitacao" id="rad_solicitacao" value="cadastrarAnalisesEmendas">Cadastrar Análises Emendas
		</td>
	</tr>
	</table>
</div>
<script type="text/javascript">
jQuery.noConflict();

function pesquisarEmenda( etoid ){
	var form = document.getElementById('formulario');
	form.action = '';
	form.target = '_self';
	jQuery('#requisicao').val('pesquisar');
	form.submit();
}

function enviarSiop(){

	jQuery( "#dialog_recurso" ).show();
	//jQuery( "#mostraRetornoRecurso" ).html('teste');
	jQuery( '#dialog_recurso' ).dialog({
			resizable: false,
			width: 350,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Enviar para SIOP': function() {
					//wssolicitacao
					
					if(jQuery('#usersiop').val() == ''){
						alert('Informe o Usuário');
						return false;
					} else if(jQuery('#passsiop').val() == ''){
						alert('Informe Senha');
						return false;
					} else if(jQuery('[name="rad_solicitacao"]:checked').length == '0'){
						alert('Informe o tipo de solicitação');
						return false;
					} else if(jQuery('[name="chk[]"]:checked').length == '0'){
						alert('Selecione uma emenda');
						return false;
					} else {
						jQuery('#requisicao').val('enviar');
						jQuery('#wsusuario').val( jQuery('#usersiop').val() );
						jQuery('#wssenha').val( jQuery('#passsiop').val() );
						jQuery('#wsperfil').val( jQuery('#userperfil').val() );
						jQuery('#wsmomento').val( jQuery('#usermomento').val() );
						jQuery('#wssolicitacao').val( jQuery('[name="rad_solicitacao"]:checked').val() );
						//jQuery('#formulario').submit();
						
						var form = document.getElementById('formulario');
						form.target = 'impositivo';
						form.action = 'emenda.php?modulo=sistema/tabelasapoio/resultEmendaImpositivoSiop&acao=A';
						var janela = window.open('emenda.php?modulo=sistema/tabelasapoio/resultEmendaImpositivoSiop&acao=A','impositivo','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
						janela.focus();
						form.submit();
						
						jQuery( this ).dialog( 'close' );
					}
				},
				'Cancelar': function() {
					jQuery( this ).dialog( 'close' );
				}
			}
	});
}

function visualizarLog(){
	if(jQuery('[name="chk[]"]:checked').length == '0'){
		alert('Selecione uma emenda');
		return false;
	} else {
		jQuery('#requisicao').val('visualizarlog');
		
		var form = document.getElementById('formulario');
		form.target = 'impositivo';
		form.action = 'emenda.php?modulo=sistema/tabelasapoio/resultEmendaImpositivoSiop&acao=A';
		var janela = window.open('emenda.php?modulo=sistema/tabelasapoio/resultEmendaImpositivoSiop&acao=A','impositivo','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
}
</script>