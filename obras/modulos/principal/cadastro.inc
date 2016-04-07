<?php
ini_set("memory_limit","1000M");

if(isset($_POST['removeAnexo'])){

	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec('arqsituacaodominial','arqid','obras');
	if($file->setRemoveUpload($_REQUEST['id'])){
		echo "true";
	}
	die;
}

if ( (integer)$_REQUEST['ajax'] == 4 ){
	echo obras_busca_campus( $_REQUEST['entid'], $_REQUEST['orgid'] );
	die;
}

if( $_REQUEST['ajaxCarregaPrograma'] )
{
	$wh = "";
	if( $_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' )
	{
		$wh = " AND prfid IN (SELECT prfid FROM obras.tipologiaprograma WHERE tpoid = ".$_REQUEST['tpoid'].") ";
	}
	
	$sql = "SELECT
				prfid as codigo,
				prfdesc as descricao
			FROM 
	 			obras.programafonte
	 		WHERE
	 			orgid = ".$_REQUEST["orgid"]."
	 			".$wh;
	
	die( $db->monta_combo("prfid", $sql, ( ( /*$bloqueiaprg == "N" ||*/ $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura ), "Selecione...", '', '', '', '340', 'S', 'prfid') );
}
if( $_REQUEST['ajaxCarregaModalidade'] )
{
	$wh = "";
	if( $_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' )
	{
		$wh = " AND moeid IN (SELECT moeid FROM obras.tipologiamodalidade WHERE tpoid = ".$_REQUEST['tpoid'].") ";
	}
	
	$sql = "SELECT 
				moeid as codigo, 
				moedsc as descricao 
			FROM 
				obras.modalidadeensino 
			WHERE 
				moestatus = 'A'
				".$wh."
			ORDER BY 
				moedsc ASC";
	die( $db->monta_combo("moeid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'moeid') );
}

if( $_REQUEST['ajaxCarregaTipoObra'] )
{
	$wh = "";
	if( $_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' )
	{
		$wh = " AND tobaid IN (SELECT tobaid FROM obras.tipologiatipoobra WHERE tpoid = ".$_REQUEST['tpoid'].") ";
	}
	
	$sql = "SELECT 
				tobaid AS codigo, 
				tobadesc AS descricao 
			FROM 
				obras.tipoobra
			WHERE
				tobstatus = 'A'
				".$wh."
			ORDER BY
				tobadesc ASC";
	
	die( $db->monta_combo("tobraid", $sql, ( $bloqueadotipo == 'N' ? $bloqueadotipo : $somenteLeitura ) , "Selecione...", '', '', '', '340', 'S', 'tobraid') );
}

if( $_REQUEST['ajaxCarregaFonte'] )
{
	$wh = "";
	if( $_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' )
	{
		$wh = " AND tooid IN (SELECT tooid FROM obras.tipologiaorigem WHERE tpoid = ".$_REQUEST['tpoid'].") ";
	}
	
	$sql = "SELECT
				tooid AS codigo,
				toodescricao AS descricao
			FROM
				obras.tipoorigemobra
			WHERE
				toostatus = 'A'
				".$wh."
			ORDER BY
				toodescricao ASC";
	
	die( $db->monta_combo("tooid", $sql, ( ( /*$bloqueiaprg == "N" ||*/ $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura ), "Selecione...", '', '', '', '340', 'S', 'tooid') );
}

if( $_REQUEST['ajaxCarregaDescricao'] )
{
	$habil = 'S';

	if( $_REQUEST['tpoid'] && $_REQUEST['tpoid'] != '' )
	{
		$obrcomposicao = $db->pegaUm("SELECT tpodetalhe FROM obras.tipologiaobra WHERE tpoid = ".$_REQUEST['tpoid']."");
		$habil = 'N';
	}
	
	$desc_composicao = "Fazer descrição funcional da obra na unidade especificando a área construída e os ambientes disponibilizados, além de outras informações relevantes.<br/><br/>Exemplo:<br/><br/>Bloco B<br/>Área Construída: 12.734 m²<br/>22 salas de aula com 1362 lugares<br/>08 laboratórios de T.I. com 248 lugares<br/>01 auditório para 60 pessoas<br/>Capacidade para atender cerca de 3200 alunos (matutino e noturno)<br/>Ar condicionado central para todas as salas";
	
	die( campo_textarea( 'obrcomposicao', 'S', $habil, '', '68', '6', '1500', '' , 0, $desc_composicao) );
}

if ( $_GET['AJAX'] ) {
	switch ( $_GET['subAcao'] ){
		case 'ajaxCarregaTipologia' : carregaTipologia( $_GET['cloid'], $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Prog' : carregaTipologiaProg( $_GET['prfid'] ); break;
		case 'ajaxCarregaTipologia_Class' : carregaTipologiaClass( $_GET['cloid'] ); break;
	}
	die;
}
// Inclusão do arquivo de permissões (somente no módulo de obras)
if ($_SESSION["sisid"] == ID_OBRAS){
	require_once APPRAIZ . 'includes/cabecalho.inc';
	require_once APPRAIZ . "www/obras/permissoes.php";
}

// Inclusão de arquivos padrão do SIMEC
require_once APPRAIZ . 'includes/Agrupador.php';

// Inclusão de arquivos do componente de Entidade
require_once APPRAIZ . "adodb/adodb.inc.php";
require_once APPRAIZ . "includes/ActiveRecord/ActiveRecord.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Endereco.php";
require_once APPRAIZ . "includes/ActiveRecord/classes/Entidade.php";

// Pega o caminho atual do usuário (em qual módulo se encontra)
$caminho_atual = $_SERVER["REQUEST_URI"];
$posicao_caminho = strpos($caminho_atual, 'acao');
$caminho_atual = substr($caminho_atual, 0 , $posicao_caminho);

// Pega url para os js
$posicao_caminho_js = strpos($caminho_atual, '?');
$caminho_atual_js   = substr($caminho_atual, 0 , $posicao_caminho_js);

$obras = new Obras();
$infraestrutura = new DadosInfraEstrutura();

switch ($_REQUEST["requisicao"]){
	case "cadastro": 
		$obras->CadastrarObras( $_REQUEST ); 
		break;
	case "atualiza":
		$obras->EnviarArquivoSituacaoDominial();
		$obras->AtualizarObras( $_REQUEST );		 
		break;
}

$dobras = new DadosObra(null);
$requisicao = "cadastro";

echo "<br/>";

if ( $_REQUEST['subAcao'] == 'novaObra' ) {
	unset( $_SESSION['obra'] );
	unset( $_REQUEST['obrid'] );
}else{
	if( $_REQUEST["obrid"] ){
	
		// Verifica se existe a obra e se o usuário possui permissão
		include_once APPRAIZ . "www/obras/_permissoes_obras.php";
	
		
		$_SESSION["obrasbkp"]["filtrosbkp"] = $_SESSION["obras"]["filtros"];
		$_SESSION["obrasbkp"]["orgid"]      = $_SESSION["obras"]["orgid"];
		$_SESSION["obrasbkp"]["ordem"]      = $_SESSION["obras"]["ordem"];
		$_SESSION["obrasbkp"]["lista"]      = $_SESSION["obras"]["lista"];
	
		// Cria a sessão com a nova obra
		unset($_SESSION["obra"]);
		unset($_SESSION["obras"]);
		$_SESSION["obra"]["obrid"]  = $_REQUEST["obrid"];
		$_SESSION["obras"]["obrid"] = $_REQUEST["obrid"];
	
	}elseif(!$_SESSION["obra"]["obrid"]) {
		$tr_campus = 'display:none;';
		// se não existir obrid então envie o usuário para a tela de seleção de obras
		header( "location:obras.php?modulo=inicio&acao=A" );
	    exit;
	}
}


if ($_SESSION["obra"]["obrid"]){

	$tr_campus = '';
	$requisicao = "atualiza";

	// Carrega os dados da obra
	$obrid = $_SESSION["obra"]["obrid"];
	$obridorigem = obras_verifica_obra_copia( $obrid );
	//(empty($obridorigem))? $status = 'A' :  $status = 'I';
	$dados = $obras->Dados($obrid, $supvid, $status);
	$dobras = new DadosObra($dados);

	$resultado = $infraestrutura->busca($_SESSION['obra']['obrid']);
	$dados = $infraestrutura->dados($resultado);


	// For uma obra do FNDE não possui campus
	if ($dobras->orgid == ORGAO_FNDE || $dobras->orgid == ORGAO_ADM || $dobras->orgid == ORGAO_REHUF || $dobras->orgid == ORGAO_MILITAR) {
		$tr_campus = 'display:none;';
	}
	if($_REQUEST["org"]) {
		$orgid = $_REQUEST["org"];
	} elseif($dobras->orgid) {
		$orgid = $dobras->orgid;
	} else {
		$orgid = $res[0]['id'];
	}
		
	$_SESSION["obra"]["orgid"]  = $orgid;
	$_SESSION["obras"]["orgid"] = $orgid;
	// Monta as abas
	montaAbaObras($abacod_tela,$url,$parametros);
	
	$sql = "select * from obras.obrainfraestrutura where obrid = ".$_SESSION["obra"]["obrid"];
	$rsObra = $db->pegaLinha($sql);
	
} else {
	if($_REQUEST["org"]) {
		$orgid = $_REQUEST["org"];
	}
}

// Cria o título da tela
$titulo_modulo = "Dados da Obra";
monta_titulo( $titulo_modulo, "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

/**
 * bloqueio de campos quando a obra for educação basica e a origem for PAC 2(tooid = 1), 
 * mais habilita para os perfis administrador e superusuario
 */ 
$bloqueioPAC2 = 'N';
if( !possuiPerfil( array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR) ) && $dobras->getTooId() == 1 ){
	$bloqueioPAC2 = 'S';	
}


?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>
	jQuery.noConflict();
</script>
<script src="/includes/prototype.js"></script>
<script src="/includes/entidades.js"></script>
<script src="/includes/calendario.js"></script>

<script>

function abreDadosIndigenas( cloid ){

	var titulo     = document.getElementById('tr_tituloind');
	var territorio = document.getElementById('tr_territorio');
	var povos	   = document.getElementById('tr_povos');

	if ( cloid == 4 ){
		
		if (document.selection){
			titulo.style.display 	 = 'block';
			territorio.style.display = 'block';
			povos.style.display 	 = 'block';
		}else{
			titulo.style.display 	 = 'table-row';
			territorio.style.display = 'table-row';
			povos.style.display 	 = 'table-row';
		}
		
	}else{
		titulo.style.display 	 = 'none';
		territorio.style.display = 'none';
		povos.style.display 	 = 'none';
		
	}

}

function abreInauguracao(valor){

	var inaugurada = document.getElementById('inaugurada');
	var dataprev   = document.getElementById("dtprevisao");
	var datain 	   = document.getElementById("dtinauguracao");
	
	if(valor == 1 || valor == 3){
		if (document.selection){
			inaugurada.style.display = 'block';
		}else{
			inaugurada.style.display = 'table-row';
		}
	}else{
		inaugurada.style.display = 'none';
		dataprev.style.display = 'none';
		datain.style.display   = 'none';
	}

}

function abreData(valor){
	
	var dataprev 			 = document.getElementById("dtprevisao");
	var datain 				 = document.getElementById("dtinauguracao");
	var obrdtprevinauguracao = document.getElementById("obrdtprevinauguracao");
	var obrdtinauguracao 	 = document.getElementById("obrdtinauguracao");
	
	if(valor == '' || valor == 'S'){
		
		obrdtinauguracao.value     = '';
		obrdtprevinauguracao.value = '';
		
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'N'){
		
		obrdtinauguracao.value     = '';
	
		if (document.selection){
			dataprev.style.display = 'block';
			datain.style.display   = 'none';	
		}else{
			dataprev.style.display = 'table-row';
			datain.style.display   = 'none';
		}
	}
	
	if(valor == 'I'){
	
		obrdtprevinauguracao.value = '';
	
		if (document.selection){
			dataprev.style.display = 'none';
			datain.style.display   = 'block';	
		}else{
			dataprev.style.display = 'none';
			datain.style.display   = 'table-row';
		}
	}
}

function retornaParaObraOriginal(obridoriginal) {
	window.location.href='?modulo=principal/cadastro&acao=A&obrid='+obridoriginal;
}

jQuery(function(){
	
	/*if(jQuery('#bloqueiochecklist').val() == 't' ){
		jQuery('input, textarea, select').attr('disabled', true);
	}*/
	
	jQuery('#formulario').submit(function(){
		situacao_dominial = jQuery('input[name=iexsitdominialimovelregulariza]:checked').val();
		
		//return false;		
		if(situacao_dominial == '1'){
			
			var files = 'false';
			jQuery.each(jQuery('.anexo_dominial'), function(i, obj){		
				if(obj.value != ''){
					files = 'true';
				}
			});
						
			anexos = jQuery('.remove_anexo');
			var orgidH = jQuery('input[name=orgid]').val();
			if(anexos.length == 0 && files == 'false' /*&& ( orgidH != 1 || orgidH != 2 )*/ && orgidH == 5 ){
				alert('O campo anexo é obrigatório!');
				return false;
			}
			
		}else{
				
			justificativa = jQuery('textarea[name=obrjustsitdominial]').val();
			if(justificativa == ''){
				alert('Preencha o campo justificativa!');
				jQuery('textarea[name=obrjustsitdominial]').focus();
				return false;
			}
		}
		
		if(jQuery('#aqiid').val() == 2){
			if(jQuery('#obrdtinicialperaquisimov').val() == '' || jQuery('#obrdtfinalperaquisimov').val() == ''){
				alert('O campo período de cessão é obrigátorio!');
				jQuery('#obrdtinicialperaquisimov').focus();
				return false;
			}
		}
		
		
	});

});

</script>
<form id="formulario" name="formulario" method="post"
	onSubmit="return Validacao( <? echo ((possuiPerfil( array(PERFIL_ADMINISTRADOR,PERFIL_SUPERVISORMEC) ))?"true":"false"); ?> );"
	action="<?php echo $caminho_atual; ?>acao=A" enctype="multipart/form-data"> 
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
<?php
if ($_SESSION["obra"]["obrid"]){
	$sql = "SELECT obrid FROM obras2.obras WHERE obrstatus = 'A' AND obrid_1 = {$_SESSION["obra"]["obrid"]}";
	$obrid_2 = $db->pegaUm( $sql );
	if ( $obrid_2 ){
	?>
		<script>alert('Essa obra foi migrada para o módulo de Obras 2.0 e está disponível apenas para consulta. Não devem ser registradas quaisquer alterações em seus dados neste módulo.')</script>
		<tr>
			<td class="SubTituloDireita" width="190px"><b>AVISO:</b></td>
			<td style="color: red;">
			Essa obra foi migrada para o módulo de Obras 2.0 e está disponível apenas para consulta. Não devem ser registradas quaisquer alterações em seus dados neste módulo.
			</td>
		</tr>
	<?php
	}
}
?>	
	<tr>
		<td class="SubTituloDireita" width="190px">Tipo de ensino:</td>
		<td><?php
			
		// Monta o combo de orgaos de acordo com a permissão do usuário
		$res = obras_pegarOrgaoPermitido();
		$arOrgaos = array();
                if($res){
                    foreach( $res as $arOrgao ){
                        $arOrgaos[] = array( 'codigo' => $arOrgao['id'], 'descricao' => $arOrgao['descricao']);
                    }
                }
		$db->monta_combo("orgid", $arOrgaos, 'N', "Selecione...", '', '', '', '', 'S', 'orgid');
		?> <input type="hidden" name="obrid" id="obrid" value="<?if($_SESSION["obra"]["obrid"]) echo $_SESSION["obra"]["obrid"];?>" />
		</td>
		<td rowspan="24" width="100" valign="top" align="center">
		<? 
		if ( $obrid ){
			
			$sql = "SELECT
						docid
					FROM
						obras.obrainfraestrutura
					WHERE
						obrid = ".$obrid;
			$docid = $db->pegaUm($sql);
			
			if( $docid ){
				
				$sql = "SELECT
							ed.esdid
						FROM 
							workflow.documento d
						INNER JOIN 
							workflow.estadodocumento ed ON ed.esdid = d.esdid
						WHERE
							d.docid = " . $docid;
				
				$esdid = (integer) $db->pegaUm( $sql );		
				$gpdid = buscaGrupoPelaObra( $obrid );
				if ( $esdid ){
	//			if ( $esdid == OBREMAPROVAMEC ){
	//				$docid = obrPegaDocidObra( $obrid );
	//				if ( $docid ){
	
						//verifica esdid do grupo
						if($gpdid){
							$sql = "SELECT
										d.esdid
									FROM 
										obras.grupodistribuicao g
									INNER JOIN 
										workflow.documento d ON d.docid = g.docid
									WHERE
										g.gpdid = " . $gpdid;
							
							$esdidGrupo = (integer) $db->pegaUm( $sql );
						}
						
						if($esdidGrupo != GRUPOAGUARDANDOINICIOSUPERVISAO){
							
							wf_desenhaBarraNavegacao( $docid , array( 'obrid' => $obrid, 'gpdid' => $gpdid) );
							
						}
	//				}
				}
			}
			$sql = " select 
						 obridorigem 
					 from 
					 	 obras.obrainfraestrutura 
				 	 where 
				 	 	 obrid = ".$obrid;
			
			$obridorigem = $db->pegaUm($sql);
			if($obridorigem){
				?>
					<table border="0" cellpadding="3" cellspacing="0" style="background-color: #f5f5f5; border: 2px solid #c9c9c9; width: 80px;">
						<tr style="background-color: #c9c9c9; text-align:center;">
							<td style="font-size:7pt; text-align:center;">
								<b>Cópia da Obra</b>
							</td>
						</tr>
						<tr style="text-align:center;">
							<td style="font-size:7pt; text-align:center;">
								<?=$obridorigem;?>
							</td>
						</tr>
							<tr style="background-color: #c9c9c9; text-align:center;">
							<td style="font-size:7pt; text-align:center;">
				
								<span title="estado atual">
									<b>ações</b>
								</span>
							</td>
						</tr>

						<tr>
						<td style="font-size: 7pt; text-align: center; border-top: 2px solid #d0d0d0;">
							<a	href="#"
								alt="Retornar para Obra Original"
								title="Retornar para Obra Original"
								onclick="retornaParaObraOriginal('<?=$obridorigem;?>')"
							><img align=absmiddle src=../imagens/workflow/3.png border=0><br/> Retornar para Obra Original</a>
						</td>
						</tr>
						</table>
				<?
			}
		}
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Unidade Implantadora:</td>
		<td><?php 
		$entid_permitidas = obras_pegarUnidadesPermitidas();
			
		if ( !$_SESSION["obra"]["obrid"] ){

			if ( $entid_permitidas ){
					
				$sql = "SELECT
							e.entid as codigo,
							e.entnome as descricao
						FROM
							entidade.entidade e
						INNER JOIN entidade.funcaoentidade fue ON fue.entid = e.entid
						INNER JOIN obras.orgaofuncao orf ON orf.funid = fue.funid
						WHERE
							e.entid in ( ". implode( ", ", $entid_permitidas ) ." ) AND orf.orgid = ".$_REQUEST["org"];
//					ver($sql);
				$db->monta_combo("entid", $sql, 'S', "Selecione...", 'abreCampus(this.value, '.$orgid.');', '', '', '340', 'S', 'entid');

			}else{
					
				$entidade = new Entidade($dobras->getEntIdUnidade());
				$entnome  = $entidade->entnome;
				$entid    = $entidade->getPrimaryKey();
				?> <input type="hidden" name="entid" id="entid"
			value="<? if(isset($obrid)) echo $entid; ?>"> <?
			}

		}else{

			$entidade = new Entidade($dobras->getEntIdUnidade());
			$entnome = $entidade->entnome;
			$entid   = $entidade->getPrimaryKey();
			?> <input type="hidden" name="entid" id="entid"
			value="<? if(isset($obrid)) echo $entid; ?>"> <?
	
			if ($_SESSION['usucpf'] <> ''){
				if ( verificaTemPreID()){
					$somenteLeitura = "N";
				}
			}
		}
		?> <span id="entnome"><?php echo $entnome; ?> <input type="hidden"
			name="entnome_h" id="entnome_h" value="<?php echo $entnome; ?>"></span>
			<?php if( $habilitado && !$entid_permitidas ){ ?> <input
			type="button" name="pesquisar_entidade" value="Pesquisar"
			style="cursor: pointer;"
			onclick="inserirEntidade(document.getElementById('entid').value,document.getElementById('orgid').value);"
			<?php if($somenteLeitura=="N") echo "disabled"; ?>> <img
			src="../imagens/obrig.gif" title="Indica campo obrigatório."
			border="0"> <?php } ?></td>
	</tr>
	
	<tr id="campus" style="<?php echo $tr_campus; ?>">
		<td class="SubTituloDireita">Campus / Reitoria:</td>
		<td id="mostracampus"><?php
		$campus = new Entidade($dobras->getEntIdCampus());
		$campusnome = $campus->entnome;
		$entidcampus = $campus->getPrimaryKey();

		if ($_SESSION["obra"]["obrid"] && $entid){
			switch ($dobras->orgid){
				case ORGAO_SESU:
					$orgaoSesu  = 'AND ef.funid = 18';
					$orgaoSesu2 = 'AND ef.funid = 75';
					break;
				case ORGAO_SETEC:
					$orgaoSesu  = 'AND ef.funid = 17';
					$orgaoSesu2 = 'AND ef.funid = 75';
					break;
				case ORGAO_REHUF:
					$orgaoSesu  = 'AND ef.funid = '.ID_HOSPITAL;
					$orgaoSesu2 = 'AND ef.funid = '.ID_HOSPITAL;
					break;
					

			}

			$sql = "
							SELECT
								et.entid as codigo,
								entnome as descricao
							FROM
								entidade.entidade et
							INNER JOIN
								entidade.funcaoentidade ef ON et.entid = ef.entid
							INNER JOIN
								entidade.funentassoc ea ON ea.fueid = ef.fueid 
							WHERE
								((ea.entid = {$entid} " . $orgaoSesu .  ") OR
								(ea.entid = {$entid} " . $orgaoSesu2 . ")) AND
								et.entstatus = 'A'
							ORDER BY descricao";


				
			$db->monta_combo("entidcampus", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'entidcampus');
		}

		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Esfera:</td>
		<td><? 
		$obrtipoesfera = $dobras->getObrTipoEsfera();
		?> 
		<?php
		if( !is_null($obrtipoesfera) && $obrtipoesfera != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !possuiPerfil( array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC, PERFIL_EMPRESA)) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "SELECT 'M' as codigo, 'Municipal' as descricao
				UNION ALL
				SELECT 'E' as codigo, 'Estadual' as descricao
				UNION ALL
				SELECT 'F' as codigo, 'Federal' as descricao";
		$db->monta_combo("obrtipoesfera", $sql, $somenteLeitura, "Selecione...", '', '', '', '100', 'N', 'obrtipoesfera');?>
		<input type=hidden id=obrtipoesfera_title value='<? echo $obrtipoesfera; ?>'></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Nome da Obra:</td>
		<td><? 
		$obrdesc = $dobras->getObrDesc();
		$obrdesc = str_replace('"', '&#34', $obrdesc);
		?> 
		<?php
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($obrdesc) && $obrdesc != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !possuiPerfil( array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC, PERFIL_EMPRESA)) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		?>
		<?= campo_texto( 'obrdesc', 'S', $somenteLeitura, '', 65, 60, '', '', 'left', '', 0); ?>
		<input type=hidden id=obrdesc_title value='<? echo $obrdesc; ?>'></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Numero do Processo:</td>
		<td><? 
		$obrnumprocesso = $dobras->getObrProcesso();
		$obrnumprocesso = str_replace('"', '&#34', $obrnumprocesso);
		?> 
		<?php
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($obrnumprocesso) && $obrnumprocesso != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !possuiPerfil( array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC, PERFIL_EMPRESA)) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		?>
		<?= campo_texto( 'obrnumprocesso', 'N', $somenteLeitura, '', 65, 25, '', '', 'left', '', 0); ?>
		<input type=hidden id=obrdesc_title value='<? echo $obrdesc; ?>'></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Tipologia da Obra:</td>
		<td>
		<?php
		$tpoid = $dobras->getTpoId();
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($tpoid) && $tpoid != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !possuiPerfil( array(PERFIL_EMPRESA, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC) ) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		if( (int)$orgid != ORGAO_REHUF && (int)$orgid != ORGAO_SESU && (int)$orgid != ORGAO_MILITAR ) $ajaxTipologia = 'ajaxTipologia';
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "SELECT tpoid as codigo, tpodsc as descricao FROM obras.tipologiaobra WHERE tpostatus = 'A' AND orgid = ".$orgid." ORDER BY tpodsc ASC";
		$db->monta_combo("tpoid", $sql, $somenteLeitura, "Selecione...", $ajaxTipologia, '', '', '340', 'N', 'tpoid');
		?>
		</td>
	</tr>
	<tr>
		<!--<td class="SubTituloDireita">Programa / Fonte:</td>-->
		<td class="SubTituloDireita">Programa:</td>
		<td>
		<span id="span_programa">
		<?php

		/*$bloqueiaprg = "S";
		if($requisicao == "atualiza") {
			$sql_existe = "SELECT o.pliid FROM monitora.pi_obra o inner join monitora.pi_planointerno pi on o.pliid = pi.pliid WHERE o.obrid='".$_SESSION["obra"]["obrid"]."' AND pi.plistatus='A'";
			$existepi = $db->pegaUm($sql_existe);
			if($existepi) {
				$bloqueiaprg = "N";
			}
		}*/

		$bloqueadosub = 'S';
		if ( !possuiPerfil( array(PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR) ) && $orgid == ORGAO_FNDE ){
			$bloqueadosub = 'N';
		}

		/*$wh = "";
		if( $tpoid )
		{
			$wh = " AND prfid IN (SELECT prfid FROM obras.tipologiaprograma WHERE tpoid = ".$tpoid.") ";
		}*/
		
		$prfid = $dobras->getPrfId();
		$somenteLeitura = ( ( /*$bloqueiaprg == "N" ||*/ $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura );
		
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($prfid) && $prfid != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		
		/*$filtro = 'orgid = '. ($_REQUEST["org"] ? $_REQUEST["org"] : $orgid);		
		if( (integer)$orgid == ORGAO_REHUF ) $filtro = '';*/
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "SELECT
				prfid as codigo,
				prfdesc as descricao
				FROM 
		 			obras.programafonte
		 		WHERE
		 			1=1
		 			and orgid = ". ($_REQUEST["org"] ? $_REQUEST["org"] : $orgid)." 
		 			".$wh."
				ORDER BY prfdesc";
		//ajaxCarregaTipologiaProg
		$db->monta_combo("prfid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'prfid');
		
		?>
		</span>
		</td>
	</tr>
	<?php if( (integer)$orgid == ORGAO_FNDE ): ?>
	<tr>
		<td class="subTituloDireita">Modalidade de Ensino:</td>
		<td>
		<span id="span_modalidade">
		<?php 
		$wh = "";
		/*if( $tpoid )
		{
			$wh = " AND moeid IN (SELECT moeid FROM obras.tipologiamodalidade WHERE tpoid = ".$tpoid.") ";
		}*/
		
		$moeid = $dobras->getMoeid();
		
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($moeid) && $moeid != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "SELECT moeid as codigo, moedsc as descricao FROM obras.modalidadeensino WHERE moestatus = 'A' ".$wh." ORDER BY moedsc ASC";
		$db->monta_combo("moeid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'moeid');
		?>
		</span>
		</td>
	</tr>
	<?php endif; ?>
	<tr>
		<td class="SubTituloDireita">Tipo de Obra:</td>
		<td>
		<span id="span_tipoobra">
		<?php

		$bloqueadotipo = 'S';
		if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && $_SESSION["obra"]["orgid"] == ORGAO_FNDE ){
			$bloqueadotipo = 'N';
		}
			
		/*$wh = "";
		if( $tpoid )
		{
			$wh = " AND tobaid IN (SELECT tobaid FROM obras.tipologiatipoobra WHERE tpoid = ".$tpoid.") ";
		}*/
		
		$tobraid = $dobras->getTobraId();
		
		$somenteLeitura = ( $bloqueadotipo == 'N' ? $bloqueadotipo : $somenteLeitura );
		
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($tobraid) && $tobraid != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "SELECT 
					tobaid AS codigo, 
					tobadesc AS descricao 
				FROM 
					obras.tipoobra
				WHERE
					tobstatus = 'A'
					".$wh."
				ORDER BY
					tobadesc ASC";
		//abreInauguracao
		$db->monta_combo("tobraid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'tobraid');
		?>
		</span>
		</td>
	</tr>
	
	
	<?php if($_SESSION["obras"]["orgid"] == ORGAO_FNDE || $_SESSION["obras"]["orgid"] == ORGAO_REHUF){?>
	<tr>
		<td class="SubTituloDireita">Fonte:</td>
		<td>
		<span id="span_fonte">
		<?php

		/*$bloqueiaprg = "S";
		if($requisicao == "atualiza") {
			$sql_existe = "SELECT o.pliid FROM monitora.pi_obra o inner join monitora.pi_planointerno pi on o.pliid = pi.pliid WHERE o.obrid='".$_SESSION["obra"]["obrid"]."' AND pi.plistatus='A'";
			$existepi = $db->pegaUm($sql_existe);
			if($existepi) {
				$bloqueiaprg = "N";
			}
		}*/

		$bloqueadosub = 'S';
		//if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && ($orgid == ORGAO_FNDE || $orgid == ORGAO_REHUF) ){
		if ( !possuiPerfil( PERFIL_SUPERUSUARIO ) && !possuiPerfil( PERFIL_ADMINISTRADOR ) && ($orgid == ORGAO_FNDE) ){
			$bloqueadosub = 'N';
		}
		
		/*$wh = "";
		if( $tpoid )
		{
			$wh = " AND tooid IN (SELECT tooid FROM obras.tipologiaorigem WHERE tpoid = ".$tpoid.") ";
		}*/
		
		//Recupera o ID da Fonte da Tabela "obras.obrainfraestrutura" pelo campo "fntid"
		//$fntid = $dobras->getFntId();
		$tooid = $dobras->getTooId();
		
		$somenteLeitura = ( ( /*$bloqueiaprg == "N" ||*/ $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura );

		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($tooid) && $tooid != '' )
		{
			//if( (integer)$orgid == ORGAO_FNDE || (integer)$orgid == ORGAO_REHUF )
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		/*$sql = " SELECT
		         		fntid AS codigo, 
		         		fnddsc AS descricao
  				 FROM 
  				 		obras.fonte
			     WHERE
		 			orgid = ". ($_REQUEST["org"] ? $_REQUEST["org"] : $orgid);*/
		$sql = "SELECT
					tooid AS codigo,
					toodescricao AS descricao
				FROM
					obras.tipoorigemobra
				WHERE
					toostatus = 'A'
					".$wh."
				ORDER BY
					toodescricao ASC";
		
		//$db->monta_combo("fntid", $sql, ( ( $bloqueiaprg == "N" || $bloqueadosub == 'N' ) ? 'N' : $somenteLeitura ), "Selecione...", '', '', '', '340', 'S', 'fntid');
		
		$db->monta_combo("tooid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'tooid');
		
		?>
		</span>
		</td>
	</tr>
	<?php }?>
	<tr>
		<td class="SubTituloDireita">Classificação da Obra:</td>
		<td><?php
		$cloid = $dobras->getCloId();
		
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($cloid) && $cloid != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$somenteLeitura = 'N';
				}
			}
		}
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;
		$sql = "
							SELECT 
								cloid as codigo,
								clodsc as descricao
							FROM
								obras.classificacaoobra
							ORDER BY
								cloid";
			
		$db->monta_combo("cloid", $sql, $somenteLeitura, "Selecione...", 'abreDadosIndigenas( this.value ); ajaxCarregaTipologiaClass', '', '', '340', 'S', 'cloid');
		?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Descrição / Composição da Obra:</td>
		<td>
		<span id="span_descricao">
		<?php
		$obrcomposicao = $dobras->getObrComposicao();

		// Se nao tiver sido obrid assume o tpoDetalhe
		if ( empty( $dados['obrid'] ) && empty( $obrcomposicao ) ) {
			$obrcomposicao = $tpodetalhe;;
		}

		$habil = 'S';//($cloid && $prfid) ? 'N' : 'S';

		if( $tpoid && (integer)$orgid != ORGAO_REHUF && (integer)$orgid != ORGAO_SESU && (integer)$orgid != ORGAO_MILITAR)
		{
			$obrcomposicao = $db->pegaUm("SELECT tpodetalhe FROM obras.tipologiaobra WHERE tpoid = ".$tpoid."");
			$habil = 'N';
		}
		(empty($obridorigem))? $habil = $habil :  $habil = $habilitado;
		$desc_composicao = "Fazer descrição funcional da obra na unidade especificando a área construída e os ambientes disponibilizados, além de outras informações relevantes.<br/><br/>Exemplo:<br/><br/>Bloco B<br/>Área Construída: 12.734 m²<br/>22 salas de aula com 1362 lugares<br/>08 laboratórios de T.I. com 248 lugares<br/>01 auditório para 60 pessoas<br/>Capacidade para atender cerca de 3200 alunos (matutino e noturno)<br/>Ar condicionado central para todas as salas";
		
		// Regra passada pelo Mário - 31/08/2011
		if( !is_null($obrcomposicao) && $obrcomposicao != '' )
		{
			if( (integer)$orgid == ORGAO_FNDE )
			{
				if( !$db->testa_superuser() && !possuiPerfil(PERFIL_ADMINISTRADOR) && !possuiPerfil(PERFIL_SUPERVISORMEC) )
				{
					$habil = 'N';
				}
			}
		}
		$habil = $bloqueioPAC2 == 'S' ? 'N' : $habil;
		?>
		<?= campo_textarea( 'obrcomposicao', 'S', $habil, '', '68', '6', '1500', '' , 0, $desc_composicao); ?>
		</span>
		</td>
	</tr>
	<tr>
		<td class="subtitulodireita">Valor Previsto (R$):</td>
		<?php (($habilitado)?$disable = 'S': $disable = 'N');
		$disable = $bloqueioPAC2 == 'S' ? 'N' : $disable;
		?>	
		<td><?php $obrvalorprevisto = number_format($dobras->getObrValorPrevisto(), 2, ",", "."); ?>
		<?= campo_texto( 'obrvalorprevisto', 'S', $disable, '', 17, 15, '###.###.###.###,##', '', 'left', '', 0, 'id="obrcustocontrato"'); ?>
		</td>
	</tr>
	<?if( $db->testa_superuser() && (integer)$orgid == ORGAO_FNDE ){ ?>
	<tr>
		<td class="subtitulodireita">Situação Obra:</td>
		<td>
		<?		
		$sql = "select 
					stoid as codigo,
				    stodesc as descricao
				from obras.situacaoobra
				where stostatus = 'A'
				order by stodesc";
		
		$db->monta_combo("stoid", $sql, 'S', "Selecione...", '', '', '', '340', 'N', 'stoid');
		?>
		</td>
	</tr>
	<?} 
	
	if( (integer)$orgid == ORGAO_FNDE)
	{
	$mobiliario = $dobras->getObrMobiliario();
	?>
	<tr>
		<td class="subtitulodireita">
			Mobiliário
		</td>
		<td>
		 
			<input  <? echo ( ($db->testa_superuser() ||  possuiPerfil(PERFIL_ADMINISTRADOR ))  ) ?  '' : 'disabled';   ?> type = checkbox name=obrmobiliario id=obrmobiliario   value="true"  <? echo( $mobiliario == 't'?  "checked" : "" ) ?>>
		</td>
	</tr>
	<?php 
	}
	?>
	
	<tr id="tr_tituloind" style="display: none;">
		<td colspan="2">Dados Indígenas</td>
	</tr>
	<tr id="tr_territorio" style="display: none;">
		<td class="subTituloDireita">Território Étno-Educacional</td>
		<td><?php 
		$somenteLeitura = $bloqueioPAC2 == 'S' ? 'N' : $somenteLeitura;	
		$terid = $dobras->getTerid();
		$sql = "SELECT 
					terid as codigo, 
					ternome as descricao 
				FROM 
					parindigena.territorioetnoeducacional 
				ORDER BY 
					ternome";
		
		$db->monta_combo("terid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'terid');

		?></td>
	</tr>
	<tr id="tr_povos" style="display: none;">
		<td class="subTituloDireita">Povos</td>
		<td><?php 
			
		$povid = $dobras->getPovid();
		$sql = "SELECT povid as codigo, povnome as descricao FROM parindigena.povoindigena ORDER BY povnome";
		$db->monta_combo("povid", $sql, $somenteLeitura, "Selecione...", '', '', '', '340', 'S', 'povid');

		?></td>
		</td>
	</tr>
	
	<?php

	$cloid = $dobras->getCloId();
	// Terra Indígena
	$boTerraIndigena = ( $cloid == 4 ) ? true : false;
	$endereco = new Endereco($dobras->getEndId());
	$entidade->enderecos[0] = $endereco;

	$disabledPAC2 = 'disabled="disabled"';
	$classEnd     = 'CampoEstilo';
	if ( $bloqueioPAC2 == 'S' ){
		$disabledPAC2 = 'disabled="disabled"';
		$classEnd     = 'disabled';
	}
//	$disabledPAC2 = ($bloqueioPAC2 == 'S') ? 'disabled="disabled"' : '';

	$habilitadoEnd = $habilitado;
	$habilitaEnd   = $habilita;
	$classEnd      = $classEnd;
	$disabledEnd   = $disabledPAC2;
	if( (integer)$orgid == ORGAO_FNDE ){
		if ( $endereco->endid || $habilitado == false ){
			$habilitadoEnd = false;
			$habilitaEnd   = 'N';
			$classEnd      = 'disabled';
			$disabledEnd   = 'disabled="disabled"';
		}
	}else{
		$habilitadoEnd = true;
		$habilitaEnd   = 'S';
		$classEnd      = '';
		$disabledEnd   = '';
	}
	echo ''
	. '<tr>'
	. '<td colspan="2">Local da Obra</td>'
	. '</tr>'

	. '<tr>'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>CEP:</label></td>'
	. '<td>'
	. '<input type="text" name="endereco[endcep]" '.$disabledEnd.' onkeyup="this.value=mascaraglobal(\'##.###-###\', this.value);" onblur="Entidade.__getEnderecoPeloCEP(this);" class="' . $classEnd . '" id="endcep" value="' . $entidade->enderecos[0]->endcep . '" size="13" maxlength="10" />'
	. ' <img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/>'
	. '</td>'
	. '</tr>'

	. '<tr id="escolha_logradouro_id" style="display:none">'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Sugestão de Logradouro:</label></td>'
	. '<td>'
	. '<input type="text" name="endlog" '.$disabledEnd.' class="' . $classEnd . '" id="endlog" value="' . $entidade->enderecos[0]->endlog . '" size="48" />'
	. '</td>'
	. '</tr>'

	. '<tr>'
	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap">
                		<label id="lbLogadouro">';
	echo ( $boTerraIndigena === true ) ? 'Terra Indígena:' : 'Logradouro:';
	echo '</label>
                	</td>'
                	. '<td>'
                	. '<input type="text" '.$disabledEnd.' name="endereco[endlog]" class="' . $classEnd . '" id="endlogradouro" value="' . $entidade->enderecos[0]->endlog . '" size="65" />'
                	. '</td>'
                	. '</tr>'
				
                	. '<tr id="trComunidadeIndigena" ';
                	echo ( $boTerraIndigena === true ) ? '>' : 'style="display:none" >';
                	echo '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Comunidade:</label></td>'
                	. '<td>';
                	$endcomunidade = $entidade->enderecos[0]->endcomunidade;
                	echo campo_textarea( 'endcomunidade', 'S', $desabilitado2, '', 68, 4, 200);
                	echo '</td>'
                	. '</tr>'
                
                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Número:</label></td>'
                	. '<td>'
                	. '<input type="text" name="endereco[endnum]" '.$disabledEnd.' class="' . $classEnd . '" id="endnum" value="' . $entidade->enderecos[0]->endnum . '" size="13" maxlength="8" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Complemento:</label></td>'
                	. '<td>'
                	. '<input type="text" name="endereco[endcom]" '.$disabledEnd.' class="' . $classEnd . '" id="endcom" value="' . $entidade->enderecos[0]->endcom . '" size="65" maxlength="100" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Bairro:</label></td>'
                	. '<td>'
                	. '<input type="text" '.$disabledEnd.' name="endereco[endbai]" class="' . $classEnd . '" id="endbai" value="' . $entidade->enderecos[0]->endbai . '" size="20" />'
                	. '</td>'
                	. '</tr>'

                	. '<tr>'
                	. '<td align="right" class="SubTituloDireita" style="width: 150px; white-space: nowrap"><label>Município/UF: </label></td>'
                	. '<td>'
                	. '<input type="text" name="mundescricao" '.$disabledEnd.'  class="' . $classEnd . '" id="mundescricao" value="' . $entidade->enderecos[0]->getMunDescricao() . '" size="20" />'
                	. '<input type="hidden" name="endereco[muncod]" id="muncod" value="' . $entidade->enderecos[0]->muncod . '" />'
                	. '<input type="text" name="endereco[estuf]" '.$disabledEnd.' class="' . $classEnd . '" id="estuf" value="' . $entidade->enderecos[0]->estuf . '" style="width: 5ex; padding-left: 2px" />'
                	. '</td>'
                	. '</tr>';

                	?>

	<tr>
		<td>Coordenadas Geográficas</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Latitude</td>
		<td><?php 
		$medlatitude = $endereco->medlatitude;
		$latitude = explode(".", $medlatitude);
		$graulatitude = $latitude[0];
		$minlatitude = $latitude[1];
		$seglatitude = $latitude[2];
		$pololatitude = $latitude[3];
		
			
		?> <?= campo_texto( 'graulatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulatitude"'); ?>
		° <?= campo_texto( 'minlatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlatitude" '); ?>
		' <?= campo_texto( 'seglatitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglatitude" '); ?>
		'' <input type="hidden" name="pololatitude" id="pololatitude">
		<?php 
		if (trim($pololatitude) == "S") {
			echo "&nbsp;<span id=pololatitude_>S</span>"; 
		} elseif(trim($pololatitude) == "N") {
			echo "&nbsp;<span id=pololatitude_>N</span>";
		} else {
			echo "&nbsp;<span id=pololatitude_></span>";
		}
		?>
		<?php print obrigatorio(); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Longitude</td>
		<td><?php 
		$medlongitude = $endereco->medlongitude;
		$longitude = explode(".", $medlongitude);
		$graulongitude = $longitude[0];
		$minlongitude = $longitude[1];
		$seglongitude = $longitude[2];
		?> <?= campo_texto( 'graulongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="graulongitude"'); ?>
		° <?= campo_texto( 'minlongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="minlongitude"'); ?>
		' <?= campo_texto( 'seglongitude', 'N', $habilitaEnd, '', 2, 2, '##', '', 'left', '', 0, 'id="seglongitude"');  ?>
		'' &nbsp W <?php print obrigatorio(); ?></td>
	</tr>
	<?php
	#if($graulongitude != "" && $minlongitude != "" && $seglongitude != "" && $graulatitude != "" && $minlatitude != "" && $seglatitude != ""){ ?>
	<tr>
		<td class="SubTituloDireita"></td>
		<td>
			<?php
			if ( $habilitadoEnd ):
			?>
			<a href="#" onclick="abreMapa();">Visualizar / Buscar No Mapa</a>
			<?php
			endif;
			?>
			<input style="display: none;" type="text" name="endereco[endzoom]"
				id="endzoom"
				value=<? if ($entidade->enderecos[0]->endzoom ==null) echo "15"; else echo $entidade->enderecos[0]->endzoom;?>>
		</td>
	</tr>
	<?php #} ?>
	<!--  <tr>
		<td colspan="2">Situação do Imóvel <br />
		<span style="color: #dd0000; font-weight: bold;">O arquivo com o
		documento indicado em Situação Dominial deve ser inserido na aba
		Documentos (PDF).</span></td>
	</tr> -->
	<tr>
		<td class="SubTituloDireita">Tipo de Aquisição do Terreno 2</td>
		<td>
			<script>
				jQuery(function(){				
					jQuery('#aqiid').change(function(){
						if(this.value == '2'){
							jQuery('#periodo_cessao').show();
						}else{
							jQuery('#periodo_cessao').hide();
						}
					});
				});
			</script>
			<?php
			
			if( !is_null($cloid) && $cloid != '' )
			{
				if( (integer)$orgid == ORGAO_FNDE )
				{
					if( !possuiPerfil( array( PERFIL_SUPERUSUARIO, PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC) ) )
					{
						$somenteLeitura = 'N';
					}
				}
			}
	
			$aqiid = $infraestrutura->aqiid;
			(($habilitado)?$disable = 'S': $disable = 'N');	
			$sql = "SELECT
								aqiid AS codigo, 
								aqidsc AS descricao 
							FROM 
								obras.tipoaquisicaoimovel
							ORDER BY
								aqiid";
				
			$db->monta_combo("aqiid", $sql, $disable,'Selecione...', '', '', '', '', 'S', 'aqiid');
			?>
		</td>
	</tr>
	<tr <?php echo $infraestrutura->aqiid == 2 ? '' : 'style="display:none;"' ?> id="periodo_cessao">
		<td width="300px" class="SubTituloDireita">
			Período de Cessão:
		</td>
		<td>
			<?php
			$obrdtinicialperaquisimov = $rsObra['obrdtinicialperaquisimov'];
			$obrdtfinalperaquisimov = $rsObra['obrdtfinalperaquisimov'];
			?>			
			de&nbsp;
			<?= campo_data( 'obrdtinicialperaquisimov', 'N', $somenteLeitura, '', 'S' ); ?>
			&nbsp;até&nbsp;
			<?= campo_data( 'obrdtfinalperaquisimov', 'N', $somenteLeitura, '', 'S' ); ?>
		</td>
	</tr>	
	<tr>
		<td width="300px" class="SubTituloDireita">Situação Dominial já
		Regularizada?</td>
		<td>
			<script>
				jQuery(function(){				
					jQuery('input[name=iexsitdominialimovelregulariza]').click(function(){
						if(this.value == '0'){
							jQuery('#justificativa_dominial').show();
						}else{
							jQuery('#justificativa_dominial').hide();
						}
					});
				});
			</script>
			<?php
			$iexsitdominialimovelregulariza = $infraestrutura->iexsitdominialimovelregulariza;
			if ($iexsitdominialimovelregulariza == "t"){
				?> <input type="radio" name="iexsitdominialimovelregulariza"
				id="iexsitdominialimovelregulariza" value="1" checked
				<? echo $disabled ?> /> Sim <input type="radio"
				name="iexsitdominialimovelregulariza"
				id="iexsitdominialimovelregulariza" value="0" <? echo $disabled ?> />
			Não <?php }else { ?> <input type="radio"
				name="iexsitdominialimovelregulariza"
				id="iexsitdominialimovelregulariza" value="1" <? echo $disabled ?> />
			Sim <input type="radio" name="iexsitdominialimovelregulariza"
				id="iexsitdominialimovelregulariza" value="0" checked
				<? echo $disabled ?> /> Não <?php } ?> <?php print obrigatorio(); ?>
		</td>
	</tr>
	<?if( $orgid == ORGAO_REHUF ){ ?>
	<tr>
		<td colspan="3" align="center" bgcolor="#dcdcdc"><b>Anexos</b></td>
	</tr>
	<tr>
		<td colspan="3">
			<script>
				jQuery(function(){
				
					jQuery('#inserir_anexo').click(function(){		
									
						html = '<tr>';
						html += '<td align="center"><img src="../imagens/excluir.gif" class="remove_file"/></td>';
						html += '<td><input type="file" name="arquivo[]" class="anexo_dominial"/></td>';
						html += '<td><textarea cols="60" rows="2" name="arqdescricao[]"></textarea></td>';						
						html += '</tr>';
											
						jQuery('#tb_anexos').find('tbody').prepend(html);
												
					});
					
					jQuery('.remove_anexo').click(function(){
						obj = jQuery(this);
						if(confirm('Deseja excluir o anexo?')){
							jQuery.ajax({
								url: 'obras.php?modulo=principal/cadastro&acao=A',
								type: 'post',
								data: 'removeAnexo=true&id='+this.id,
								success: function(e){									
									if(e=='true'){
										obj.parent().parent().remove();
									}else{
										alert('Não foi possível deletar o anexo!');
									}
								}
							});
														
						}
					});
					
					jQuery('.remove_file').live('click',function(){						
						jQuery(this).parent().parent().remove();
					});
					
				});
			</script>
			<?php
				$sql = "select * from obras.arqsituacaodominial bb
						join public.arquivo aa on aa.arqid = bb.arqid
						where bb.obrid = ".$_SESSION["obra"]["obrid"];
				
				if( $_SESSION["obra"]["obrid"] ) $arrArquivos = $db->carregar($sql);
			?>			
			<table id="tb_anexos" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" 
			cellPadding=3 align="center" width="95%">
				<thead>				
					<tr>
						<td class="subtitulocentro" width="15%">Ação</td>
						<td class="subtitulocentro" width="55%">Arquivo</td>
						<td class="subtitulocentro" width="30%">Descrição</td>						
					</tr>
				</thead>
				<tbody>					
					<?php if($arrArquivos): ?>						
						<?php foreach($arrArquivos as $dados): ?>
							<tr id="anexo_arquivo">
								<td align="center">
									<?if($boReformulacao){ ?>
										<img src="../imagens/excluir_01.gif" class="remove_anexo" id="<?php echo $dados['arqid'] ?>"/>
									<?}else{ ?>
										<img src="../imagens/excluir.gif" class="remove_anexo" id="<?php echo $dados['arqid'] ?>"/>
									<?} ?>
								</td>
								<td>
								<a style="cursor: pointer; color: blue;" onclick="DownloadArquivo('<?php echo $dados['arqid'] ?>');" />
									<?php echo $dados['arqnome'].'.'.$dados['arqextensao'] ?>
									</a>
								</td>
								<td>
									<?php echo $dados['arqdescricao'] ?>
								</td>
							</tr>		
						<?php endforeach; ?>
					<?php endif; ?>
				</tbody>
			</table>		
		</td>
	</tr>
	<tr>
		<td colspan="3">
			<?if($habilitado){ ?>
			<a href="javascript:void(0)" id="inserir_anexo"> 
				<img src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0" title="Inserir Anexo"> 
				Inserir Anexo</a>
			<?} ?>
		</td>
	</tr>
	<?} ?>
	<tr <?php echo $iexsitdominialimovelregulariza != 't' ? '' : 'style="display:none;"' ?> id="justificativa_dominial">
		<td width="300px" class="SubTituloDireita">
			Justificativa:
		</td>
		<td>			
			<textarea name="obrjustsitdominial" cols="60" rows="6"><?php echo $rsObra['obrjustsitdominial'] ?></textarea>
		</td>
	</tr>	
	<tr>
		<td colspan="3">Sobre a Inauguração</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Inaugurada</td>
		<td colspan="2"><?php $obrstatusinauguracao = $dobras->getObrStatusInauguracao(); ?>

		<input <?=(($disable == 'N')? 'disabled=\"disabled\"': '')?>  type="radio" name="obrstatusinauguracao" id="naoseaplica"
			value="S" onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "S") echo 'checked="checked"'; ?> />
		Não se Aplica <br />
		<input <?=(($disable == 'N')? 'disabled=\"disabled\"': '')?> type="radio" name="obrstatusinauguracao" id="naoinaugurada"
			value="N" onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "N") echo 'checked="checked"'; ?> />
		Não Inaugurada <br />
		<span style="display: none;" id="inaugurada"> <input <?=(($disable == 'N')? 'disabled=\"disabled\"': '')?> type="radio"
			name="obrstatusinauguracao" id="ainaugurada" value="I"
			onclick="abreData(this.value);"
			<?php if($obrstatusinauguracao == "I") echo 'checked="checked"'; ?> />
		Inaugurada </span></td>
	</tr>
	<tr id="dtprevisao" style="display: none;">
		<td class="SubTituloDireita">Data de Previsão da Inauguração</td>
		<td><?php $obrdtprevinauguracao = $dobras->getObrDtPrevInauguracao(); ?>
		<?= campo_data( 'obrdtprevinauguracao', 'N', $somenteLeitura, '', 'S' ); ?>
		</td>
	</tr>
	<tr id="dtinauguracao" style="display: none;">
		<td class="SubTituloDireita">Data de Inauguração</td>
		<td><?php $obrdtinauguracao = $dobras->getObrDtInauguracao(); ?> <?= campo_data( 'obrdtinauguracao', 'N', $somenteLeitura, '', 'S' ); ?>
		</td>
	</tr>
	<tr>
		<td colspan="3">&nbsp;</td>
	</tr>
	
	<tr>
		<td colspan="3" align="center" bgcolor="#dcdcdc"><b>Contatos</b></td>
	</tr>
	<tr>
		<td colspan="3"><script type="text/javascript">
				<!--
					function RemoveLinha( idContato ){
						if (confirm('Deseja realmente excluir este contato?')){
							var index = window.document.getElementById('linha_'+idContato).rowIndex;
							table = window.document.getElementById("responsaveiscontato");
							table.deleteRow(index);
						}
					}
				-->
			</script>
		<table id="responsaveiscontato" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td class="subtitulocentro" width="15%">Ação</td>
				<td class="subtitulocentro" width="55%">Nome</td>
				<td class="subtitulocentro" width="30%">Categoria de Responsabilidade</td>
			</tr>
			<?php

			if($_SESSION['obra']['obrid']){
				$sql = pg_query("
									SELECT
										rc.tprcid as tipo, 
										rc.recoid as responsavel,
										rc.entid as entidade, 
										et.entnome as nome,
										et.entnumcpfcnpj as cpf  
									FROM 
										obras.responsavelobra r 
									INNER JOIN 
										obras.responsavelcontatos rc ON r.recoid = rc.recoid
									INNER JOIN 
										entidade.entidade et ON rc.entid = et.entid
									WHERE 
										r.obrid = '". $_SESSION['obra']['obrid'] . "'  AND rc.recostatus = 'A' AND et.entstatus != 'I'");

				while (($dados = pg_fetch_assoc($sql))){
					$tipo 		 = $dados["tipo"];
					$responsavel = $dados['responsavel'];
					$entidade    = $dados['entidade'];
					$id   = $dados['recoid'];
					$nome = $dados['nome'];
					$cpf  = $dados['cpf'];

					$sql2 = pg_query("
									SELECT 
										tiporesp.tprcid, 
										tiporesp.tprcdesc
									FROM 
										obras.tiporespcontato AS tiporesp");
					
					$boContatoDisa = false;
					if($somenteLeitura == "S")
					$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\" id=\"tprcid[" . $entidade . "]\">";
					else {
						$combo_responsabilidade = "<select style='width:170px' class='CampoEstilo' name=\"tprcid[" . $entidade . "]\" id=\"tprcid[" . $entidade . "]\" disabled>";
						$boContatoDisa = true;
					}

					while ($tipos = pg_fetch_array($sql2)){
							
						$selected = "";
						$tprcid = $tipos['tprcid'];
						$tprcdescricao = $tipos['tprcdesc'];
							
						if ($tipo == $tprcid){
							$selected = "selected";

						}
						$combo_responsabilidade .= "<option value='{$tprcid}' {$selected}>" . $tprcdescricao . "</option>";
					}

					$combo_responsabilidade .= "</select>";
					//if( $boContatoDisa ) $combo_responsabilidade .= "";
					
					if( possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO)) && !$boReformulacao && $habilitado ){
						$botoes = "<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" border=0 title=\"Visualizar\" onclick=\"visualizaResponsavel('" . $entidade . "')\">
									&nbsp&nbsp&nbsp<img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\" onclick=\"atualizaResponsavel('" . $entidade . "')\">
							       &nbsp&nbsp&nbsp<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(" . $entidade . ");\">";
					}else{
						$botoes = "<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" border=0 title=\"Visualizar\" onclick=\"visualizaResponsavel('" . $entidade . "')\">&nbsp&nbsp&nbsp
									<img src=\"/imagens/alterar_pb.gif\" style=\"cursor: pointer\" border=0 title=\"Editar\">&nbsp&nbsp&nbsp
								   <img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
					}

					if( possuiPerfil( array(PERFIL_SUPERVISORMEC, PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO, PERFIL_EMPRESA) ) && $habilitado ){
						$bt_envia_email = '&nbsp&nbsp&nbsp<img border="0" onclick=\'envia_email("' . $cpf . '");\' title="Enviar e-mail ao Gestor" src="../imagens/email.gif" style="cursor: pointer;"/>';
					}


					echo "
									<tr id=\"linha_" . $entidade . "\">
										<td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
										<td id=\"nomeEntidade_" . $entidade . "\">" . $codigo . $bt_envia_email . ' ' . $nome . $id . "</td>
										<td>".($boContatoDisa ? "<input type=\"hidden\"  value=\"".$tipo."\" name=\"tprcid[" . $entidade . "]\" >" : ''). 
												$combo_responsabilidade . "</td>
									</tr>";
				}
			}
			?>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="3"><?php if($habilitado){ ?> <a href="#"
			onclick="inserirResponsavel(); return false;"> <img
			src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0"
			title="Inserir Contatos"> Inserir Contatos </a> <?php } ?></td>
	</tr>

	<?php if(  in_array($_SESSION["obra"]["orgid"], array(ORGAO_FNDE, ORGAO_SETEC, ORGAO_SESU, ORGAO_MILITAR)) ){ ?>

	<tr>
		<td colspan="3" align="center" bgcolor="#dcdcdc"><b>Responsáveis pela Obra</b></td>
	</tr>
	<tr>
		<td colspan="3">
		<table id="responsaveisobra" class="tabela" bgcolor="#f5f5f5"
			cellSpacing="1" cellPadding=3 align="center" width="95%">
			<tr>
				<td class="subtitulocentro" width="15%">Ação</td>
				<td class="subtitulocentro" width="30%">CPF</td>
				<td class="subtitulocentro" width="55%">Nome</td>
			</tr>
			<?php litaResponsavelObra(); ?>
		</table>
		</td>
	</tr>
	<tr>
		<td colspan="3"><?php if($habilitado && possuiPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_SUPERUSUARIO)) ){ ?>
		<a href="#"
			onclick="inserirEditorObras(<?=$dobras->getEntIdUnidade()?>);"> <img
			src="/imagens/gif_inclui.gif" style="cursor: pointer;" border="0"
			title="Associar Responsáveis"> Associar Responsáveis </a> <?php } ?>
		</td>
	</tr>

	<?php } ?>

	<tr>
		<td class="SubTituloDireita">Observação sobre a Obra:</td>
		<td colspan="2" ><? $obsobra = stripcslashes($dobras->getObsObra()); ?> <?= campo_textarea( 'obsobra', 'N', $somenteLeitura, '', '80', '6', '1000'); ?>
		</td>
	</tr>
	
	
	<tr bgcolor="#C0C0C0">
		<td></td>
		<td colspan="2">
		<?
		if( $dobras->obroriginal == 't' ){
			if( possuiPerfil(array(PERFIL_ADMINISTRADOR, PERFIL_SUPERVISORMEC, PERFIL_SUPERUSUARIO, PERFIL_EMPRESA)) ){
				$somenteLeitura = 'S';
			}
		}
		?>
		<div style="float: left;">
			<input type="hidden" name="requisicao" value="<?php echo $requisicao; ?>" /> 
			<input type="submit" value="Salvar" style="cursor: pointer" <?php if( ($somenteLeitura=="N") || $boReformulacao || $boBloqueioAba ) echo "disabled"; ?>> 
			<input type="button" value="Voltar" style="cursor: pointer" onclick="history.back(-1);"></div>
		</td>
	</tr>
</table>
</form>

<script>abreDadosIndigenas(<?php print $dobras->getCloId()?>);</script>
			<?php

			if( $dobras->getStoId() == EM_CONSTRUCAO || $dobras->getStoId() == FINALIZADA ){
				echo "
		<script>
			
			if (document.selection){
				document.getElementById('inaugurada').style.display = 'block';
			}else{
				document.getElementById('inaugurada').style.display = 'table-row';
			}

			abreData('" . $obrstatusinauguracao . "');
		</script>";
			}
			?>
			<?php chkSituacaoObra();?>
			<?php 
				if($obrid){ 
						$gpdid = buscaGrupoPelaObra( $obrid );
						tramitaGrupo($gpdid); 
				}
			?>
