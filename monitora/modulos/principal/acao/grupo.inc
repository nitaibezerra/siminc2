<?php

/*

insert into menu (
	mnucod,mnuidpai,mnudsc,mnutransacao,mnulink,mnutipo,mnustile,mnuhtml,mnusnsubmenu,mnushow,mnustatus,abacod,sisid
) values (
	1298,26,'Cadastrar Grupos de Ação','Cadastrar Grupos de Ação','monitora.php?modulo=principal/acao/grupotipo&acao=A','3','','','f','t','A',null,1
);

create table monitora.grupo (
	gruid serial not null,
	grudsc varchar(255) not null,
	constraint pk_grupo primary key ( gruid )
) with oids;

create table monitora.acaogrupo (
	acaid int4 not null,
	gruid int4 not null,
	constraint pk_acaogrupo primary key ( acaid, gruid ),
	constraint fk_acaogrupo_acaid foreign key ( acaid )
		references monitora.acao ( acaid ) match simple
		on update cascade
		on delete restrict,
	constraint fk_acaogrupo_gruid foreign key ( gruid )
		references monitora.grupo ( gruid ) match simple
		on update cascade
		on delete restrict
) with oids;

*/

$sql = sprintf(
	"select count( pflcod )
	from seguranca.perfilusuario pu
	where pu.usucpf = '%s' and  pu.pflcod in ( 18, 112, 6 )",
	$_SESSION['usucpf']
);
$privilegio = $db->pegaUm( $sql ) > 0;
$habil = $privilegio ? 'S' : 'N';

if ( $privilegio && $_REQUEST['grupo'] ) {
	if ( $_REQUEST['gruid'] && $_REQUEST['excluir'] ) {
		$sql = sprintf( "delete from monitora.acaogrupo where gruid = %d", $_REQUEST['gruid'] );
		$db->executar( $sql );
		$sql = sprintf( "delete from monitora.grupo where gruid = %d", $_REQUEST['gruid'] );
		$db->executar( $sql );
		unset( $_REQUEST['gruid'] );
	} else {
		if ( empty( $_REQUEST['gruid'] ) ) {
			$sql = sprintf( "insert into monitora.grupo ( grudsc ) values ( '%s' )", $_REQUEST['grudsc'] );
		} else {
			$sql = sprintf( "update monitora.grupo set grudsc = '%s' where gruid = %d", $_REQUEST['grudsc'], $_REQUEST['gruid'] );
			# grupos
			$sql_grupo = sprintf( "delete from monitora.acaogrupo where gruid = %d", $_REQUEST['gruid'] );
			$db->executar( $sql_grupo );
			foreach( (array) $_REQUEST['acaid'] as $acao ){
				$sql_grupoacao = sprintf(
					"insert into monitora.acaogrupo ( gruid, acaid ) values ( %d, %d )",
					$_REQUEST['gruid'],
					$acao
				);
				$db->executar( $sql_grupoacao );
			}
		}
		$db->executar( $sql );
	}
	$db->commit();
	$db->sucesso( $_REQUEST['modulo'] );
}

if ( $_REQUEST['gruid'] ) {
	$sql = sprintf( "select gruid, grudsc from grupo where gruid = %d", $_REQUEST['gruid'] );
	$grupo = $db->pegaLinha( $sql );
}

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

?>

<div id="grupo" style="display: none; margin: 0 0 20px 0;">
	<form action="" method="post" name="grupo">
		<input type="hidden" name="grupo" value="1"/>
		<input id="gruid" type="hidden" name="gruid" value=""/>
		<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Grupo:</td>
				<td><?= campo_texto( 'grudsc', 'S', $habil, '', 60, 100, '', '' ); ?></td>
			</tr>
			<?php if( $grupo ): ?>
				<tr id="associacao">
					<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%;">Ações:</td>
					<td>
						<style>
							.normal { margin: 2px; }
							.CampoEstilo { margin: 2px; }
						</style>
						<script type="text/javascript" src="../includes/json.js"></script>
						<script type="text/javascript">
							
							var URL_AJAX = 'http://<?= $_SERVER['SERVER_NAME'] ?>/<?= $_SESSION['sisdiretorio']?>/ajax_combo_acao.php'
							var requisicao = null;
							
							function incluir_acao(){
								if ( requisicao != null ) {
									return;
								}
								// captura e filtra os dados do formulário
								var programa = document.getElementById( 'programa' );
								var acao = document.getElementById( 'acao' );
								var unidade = document.getElementById( 'unidade' );
								var localizador = document.getElementById( 'localizador' );
								if ( programa.value.length == 4 || acao.value.length == 4 || unidade.value.length == 5 || localizador.value.length == 4 ) {
									// efetiva a requisição
									var endereco = URL_AJAX
										+ '?programa=' + programa.value
										+ '&acao=' + acao.value
										+ '&unidade=' + unidade.value
										+ '&localizador=' + localizador.value;
									requisicao = window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
									requisicao.open( 'GET', endereco, true );
									requisicao.onreadystatechange = capturar;
									requisicao.send( null );
								}
							}
							
							function excluir_acao(){
								var campo = document.getElementById( 'acaid' );
								for( i = 0; i < campo.options.length; i++ ){
									if ( campo.options[i].selected ) {
										campo.options[i] = null;
										i--;
									}
								}
							}
							
							function monitorar( campo, proximo ){
								if ( campo.value.length != campo.maxLength ) {
									return;
								}
								if ( proximo ) {
									proximo.focus();
								}
							}
							
							function capturar(){
								try {
									if ( requisicao.readyState == 1 ) {
										bloquear_arvore();
									} else if ( requisicao.readyState == 4 ) {
										inserir_itens( JSON.parse( requisicao.responseText ) );
										requisicao = null;
									}
								}
								catch( e ) {}
							}
							
							function inserir_itens( lista ){
								var campo = document.getElementById( 'acaid' );
								for( i = 0; i < lista.length; i++ ){
									var codigo = lista[i]['acaid'];
									var descricao = lista[i]['prgcod'] + '.' + lista[i]['acacod'] + '.' + lista[i]['unicod'] + '.' + lista[i]['loccod'] + ' ' + lista[i]['acadsc'];
									// verifica se a ação já está na lista
									var igual = false;
									for( j = 0; j < campo.options.length; j++ ){
										if ( campo.options[j].value == codigo ) {
											igual = true;
											break;
										}
									}
									// inclui a ação
									if ( !igual ) {
										campo.options[campo.options.length] = new Option( descricao, codigo, false, false );
									}
								}
								sortSelect( campo );
							}
							
							function selecionar_opcao( campo ){
								var opcao = null;
								for( i = 0; i < campo.options.length; i++ ){
									if ( campo.options[i].selected ) {
										if ( opcao != null ) {
											document.getElementById( 'programa' ).value = '';
											document.getElementById( 'acao' ).value = '';
											document.getElementById( 'unidade' ).value = '';
											document.getElementById( 'localizador' ).value = '';
											return; 
										}
										opcao = campo.options[i];
									}
								}
								document.getElementById( 'programa' ).value = opcao.innerHTML.substring( 0, 4 );
								document.getElementById( 'acao' ).value = opcao.innerHTML.substring( 5, 9 );
								document.getElementById( 'unidade' ).value = opcao.innerHTML.substring( 10, 15 );
								document.getElementById( 'localizador' ).value = opcao.innerHTML.substring( 16, 20 );
							}
							
							function limpar_acao(){
								var campo = document.getElementById( 'acaid' );
								for( i = campo.options.length; i >= 0; i-- ){
									campo.options[i] = null;
								}
							}
							
						</script>
						<input id="programa" name="_programa" type="text" class="normal" style="width: 60px;" maxlength="4" onchange="monitorar( this, document.getElementById( 'acao' ) );"/>
						<input id="acao" name="_acao" type="text" class="normal" style="width: 60px;" maxlength="4" onchange="monitorar( this, document.getElementById( 'unidade' ) );"/>
						<input id="unidade" name="_unidade" type="text" class="normal" style="width: 60px;" maxlength="5" onchange="monitorar( this, document.getElementById( 'localizador' ) );"/>
						<input id="localizador" name="_localizador" type="text" class="normal" style="width: 60px;" maxlength="4" onchange="monitorar( this, document.getElementById( 'incluir' ) );"/>
						<input id="incluir" type="button" name="botao" value="+" onclick="incluir_acao();"/>
						<input id="excluir" type="button" name="botao" value="-" onclick="excluir_acao();"/>
						<br/>
						<?php
							$sql = sprintf(
								"select a.acaid, a.prgcod, a.acacod, a.unicod, a.loccod, a.acadsc
								from monitora.acaogrupo ag
								inner join monitora.acao a on a.acaid = ag.acaid
								where ag.gruid = %d
								order by a.prgcod, a.acacod, a.unicod, a.loccod, a.acadsc",
								$_REQUEST['gruid']
							);
							$acoes = $db->carregar( $sql );
							$acoes = $acoes ? $acoes : array();
						?>
						<select id="acaid" name="acaid[]" multiple="multiple" class="CampoEstilo" style="width: 330px; height: 100px;" onchange="selecionar_opcao( this );">
							<?php foreach( $acoes as $acao ): ?>
							<option value="<?= $acao['acaid'] ?>"><?= $acao['prgcod'] .'.'. $acao['acacod'] .'.'. $acao['unicod'] .'.'. $acao['loccod'] .' '. $acao['acadsc']?></option>
							<?php endforeach; ?>
						</select>
						<br/>
						<input id="excluir" type="button" name="botao" value="Excluir Todas" onclick="limpar_acao();"/>
						<p>&nbsp;</p>
					</td>
				</tr>
			<?php endif; ?>
			<?php if( $privilegio ): ?>
				<tr style="background-color: #cccccc">
					<td align='right' style="vertical-align:top; width:25%;">&nbsp;</td>
					<td>
						<input type="button" name="botao" value="Gravar" onclick="gravar_grupo();"/>
						<!--
						<?php if( $_REQUEST['gruid'] ): ?>
							<input type="button" name="botao" value="Excluir" onclick="excluir_grupo();"/>
						<?php endif; ?>
						-->
						<input type="button" name="cancelar" value="Cancelar" onclick="desativar_formulario();"/>
					</td>
				</tr>
			<?php endif; ?>
		</table>
	</form>
</div>

<table align="center" class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td colspan="2" class="SubTituloDireita">
			<input id="botao_grupo" type="button" value="Cadastrar Grupo" onclick="ativar_formulario();"/>
		</td>
	</tr>
</table>

<?php

$sql = sprintf(
	"select
		'<img border=\"0\" src=\"../imagens/alterar.gif\" title=\"Alterar Grupo\" onclick=\"selecionar_grupo(' || gruid || ');\" style=\"cursor: pointer;\">
		&nbsp;&nbsp;&nbsp;
		<img border=\"0\" src=\"../imagens/excluir.gif\" title=\"Excluir Grupo\" onclick=\"excluir_grupo(' || gruid || ');\" style=\"cursor: pointer;\">'
				as acao,
		grudsc
	from grupo
	order by grudsc"
);
$cabecalho = array( "Ação", "Grupo" );
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', '', '' );

?>

<script>

	function selecionar_todas_acoes() {
		var campo = document.getElementById( 'acaid' );
		if ( campo ) {
			for ( var i = 0; i < campo.length; i++ ) {
				campo.options[i].selected = true;
			}
		}
	}
	
	function ativar_formulario(){
		document.getElementById( 'grupo' ).style.display = 'block';
		document.getElementById( 'botao_grupo' ).disabled = true;
	}
	
	function desativar_formulario(){
		document.grupo.grudsc.value = '';
		document.grupo.gruid.value = '';
		document.getElementById( 'grupo' ).style.display = 'none';
		document.getElementById( 'associacao' ).style.display = 'none';
		document.getElementById( 'botao_grupo' ).disabled = false;
	}
	
	function gravar_grupo(){
		var validacao = true;
		var mensagem = '';
		if ( document.grupo.grudsc.value == '' ) {
			mensagem += 'É necessário informar o nome do grupo.';
			validacao = false;
		}
		if ( !validacao ) {
			alert( mensagem );
			return;
		}
		selecionar_todas_acoes();
		document.grupo.submit();
	}
	
	function selecionar_grupo( gruid, grudsc ){
		window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&gruid=' + gruid;
	}
	
	function excluir_grupo( gruid ){
		if ( !confirm( 'Você deseja excluir o grupo?' ) ) {
			return;
		}
		if ( !gruid ) {
			var gruid = document.getElementById( 'gruid' ).value;
		}
		window.location = '?modulo=<?= $_REQUEST['modulo'] ?>&acao=<?= $_REQUEST['acao'] ?>&gruid=' + gruid + '&excluir=1&grupo=1';
	}
	
	<?php if( $grupo ): ?>
		document.grupo.grudsc.value = '<?= $grupo['grudsc'] ?>';
		document.grupo.gruid.value = <?= $grupo['gruid'] ?>;
		ativar_formulario();
	<?php endif; ?>
	
</script>