<?php 
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

switch ( $_POST['op'] ){
	case 'apagar':
		$empreendimento = new Empreendimento( $_POST['empid'] );
		if ( $empreendimento->empid ){
			$empreendimento->empstatus = 'I';
			$empreendimento->salvar();
		}
		$db->commit();
		die('<script type="text/javascript">
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/listaEmpreendimentos&acao=A\';
			 </script>');
		
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgaoemprendimento');
echo montarAbasArray($arAba, "?modulo=principal/listaEmpreendimentos&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Empreendimentos', 'Filtre os Empreendimentos');
//cria_abas_Obras(ID_ABA_LISTA_EMP,$url,$parametros);
//monta_titulo_listaObras();

extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$('.pesquisar').click(function(){
		$('#formListaObra').submit();
	});

	$('.novaObra').click(function(){
		window.location.href = 'obras2.php?modulo=principal/cadEmpreendimento&acao=A';
	});

	$('.novoObjeto').click(function(){
		window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A&empid='+$(this).attr('id');
	});
	
<?php
if ( $abreBuscaAvancada ){
	echo "exibeBuscaAvancada( " . ($abreBuscaAvancada == 't' ? 'true' : 'false') . " )";	
}
?>
	
});

function exibeBuscaAvancada(visivel){
	if ( visivel == true ){
		$('#tr_busca_avancada').show();
		$('#labelBuscaAvancada').hide();
		$('#abreBuscaAvancada').val('t');
	}else{
		$('#tr_busca_avancada').hide();
		$('#labelBuscaAvancada').show();
		$('#abreBuscaAvancada').val('f');
	} 	
}

function carregarMunicipio( estuf ){
					var td	= $('#td_municipio');
					if ( estuf != '' ){
						var url = location.href;
						$.ajax({
									  url  		 : url,
									  type 		 : 'post',
									  data 		 : {ajax  : 'municipio', 
									  		  	    estuf : estuf},
									  dataType   : "html",
									  async		 : false,
									  beforeSend : function (){
									  	divCarregando();
										td.find('select option:first').attr('selected', true);
									  },
									  error 	 : function (){
									  	divCarregado();
									  },
									  success	 : function ( data ){
									  	td.html( data );
									  	divCarregado();
									  }
								});	
					}else{
						td.find('select option:first').attr('selected', true);
						td.find('select').attr('selected', true)
										 .attr('disabled', true);
					}			
}

function alterarEmp( empid ){
	location.href = '?modulo=principal/cadEmpreendimento&acao=A&empid=' + empid;
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function resumoEmp( empid ){
	location.href = '?modulo=principal/mobile_gerencial_infraestrutura&acao=A&empid=' + empid;
}

function excluirEmp( empid ){
	if ( confirm('Deseja apagar esta empreendimento?') ){
		$('#empid').val(empid);
		$('#op').val('apagar');
		$('#formListaObra').submit();
	}
}
</script>
<form method="post" name="formListaObra" id="formListaObra">
	<input type="hidden" name="op" id="op" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome do Empreendimento / ID:</td>
			<td>
				<?=campo_texto('empbuscatexto','N','S','',70,100,'','', '', '', '', 'id="empbuscatexto"', '', $_REQUEST['empbuscatexto']);?>
				&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
				<a href="javascript:exibeBuscaAvancada( true );" id="labelBuscaAvancada">[Busca avançada]</a>
			</td>
		</tr>
		<tr>
			<td id="tr_busca_avancada" colspan="2" style="display: none;">
				<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
					<tr>
						<th colspan="2">
							Busca Avançada
							<a style="float:right;" onclick="exibeBuscaAvancada( false );">[Fechar]</a>
						</th>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipo do Empreendimento:</td>
						<td>
							<?php 
							$tipoObra = new TipoObra();
							$db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid" );
							?>
						</td>
					</tr>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Classificação do Empreendimento:</td>
						<td>
							<?php 
							$classificacaoObra = new ClassificacaoObra();				
							$db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid" );
							?>
						</td>
					</tr>
				<?php 
				if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL ):
				?>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Tipologia do Empreendimento:</td>
						<td>
							<?php 
								$tipologiaObra = new TipologiaObra();	
								$param = array("orgid"   => $_SESSION['obras2']['orgid']);						
								$db->monta_combo( "tpoid", $tipologiaObra->listaCombo( $param ), "S", "Todas", "", "", "", 200, "N", "tpoid" );
							?>
						</td>
					</tr>
				<?php
				endif;
				?>
					<tr>
						<td class="SubTituloDireita" style="width: 190px;">Programa:</td>
						<td>
							<?php 
							$programa = new ProgramaFonte();				
							$db->monta_combo( "prfid", $programa->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todos", "", "", "", 200, "N", "prfid" );
							?>
						</td>
					</tr>
					
			<?php if($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
					<td>
						<?php 
						$modalidade = new ModalidadeEnsino();
						$db->monta_combo( "moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid" );
						?>
					</td>
				</tr>
			<?php endif; ?>
			
				<tr>
					<td class="SubTituloDireita">UF:</td>
					<td>
					<?php
					$uf = new Estado();					
					$db->monta_combo("estuf", $uf->listaCombo(), 'S','Selecione...','carregarMunicipio', '', '',200,'N','estuf'); 
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Município:</td>
					<td id="td_municipio">
					<?php 
					if ($estuf){
						$municipio = new Municipio();
						$dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
						$habMun    = 'S';
					}else{
						$dado   = array();
						$habMun = 'N';
					}		
					$habMun = ($disable == 'N' ? $disable : $habMun);
					echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod'); 
					?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita">Esfera:</td>
					<td>
						<?php 
						$sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),
									 Array('codigo'=>'E', 'descricao'=>'Estadual'),
									 Array('codigo'=>'F', 'descricao'=>'Federal'));
						$db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
						?>
					</td>
				</tr>
				<tr>
					<td class="SubTituloDireita" style="width: 190px;">Valor do Contrato:</td>
					<td>
						De:&nbsp;
						<?php 
						echo campo_texto( 'empvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
						?>
						Até:&nbsp;
						<?php 
						echo campo_texto( 'empvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
						?>
					</td>
				</tr>
					
					
				</table>			
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<?php
				if ( possui_perfil( PFLCOD_SUPER_USUARIO ) ):
				?>
				<div style="float:left" class="novaObra" > 
					<a style="cursor: pointer;"> <img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Novo Empreendimento"> Novo Empreendimento </a>
				</div>
				<?php
				endif;
				?>
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>
<?php 
if ( $_POST || !possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) ) ){
	$empreendimento 	= new Empreendimento();
	$param 	 		= $_POST;
//	$param['NOT(mi)'] 	= true;
	$sql 			= $empreendimento->listaEmpreendimento( $param );
	$cabecalho = array("Ação", "ID da Obra", "ID do Empreendimento","Nome do Empreendimento", "Unidade Implantadora", "UF - Município", "Data de Início", "Data de Término", "Tipologia", /*"Situação",*/ "Valor Contrato", "Data da Última Vistoria", "% Executado", "Data da Última Supervisão", "% Supervisão");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','');
}else{
?>	
	<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
		<tr>
			<td style="text-align: justify;">
				Para listar os emprendimentos, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.		
			</td>
		</tr>
	</table>		
<?php	
}
?>