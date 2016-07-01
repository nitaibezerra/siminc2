<?php

pegaSessoes(1,0,0);
$evento = array("N", "exclui_homologacao", "exclui_nomeacao", "cadastra");
if (!in_array($_REQUEST["evento"], $evento)){
	
	if (empty($_REQUEST['tpetipo'])){
		switch ($_REQUEST['acao']){
			case 'C':
				$tipoEdital = ACA_TPEDITAL_PUBLICACAO;
			break;
			case 'H':
				$tipoEdital = ACA_TPEDITAL_HOMOLOGACAO;
			break;		
			case 'N':
				$tipoEdital = ACA_TPEDITAL_NOMEACAO;
			break;		
			default:
				$tipoEdital = '';
		}
	}else{
		$tipoEdital = $_REQUEST['tpetipo'];	
	}
	verificaEditalPortaria($_GET['edpid'] ? $_GET['edpid'] : $_SESSION["academico"]["edpid"], $tipoEdital);
}
// verifica se existe o edital
if ( $_REQUEST["edpid"] ){
	$existe_edital = academico_existeedital( $_REQUEST["edpid"] );
	
	if ( !$existe_edital ){
		echo "<script>
				alert('O edital informado não existe!');
				history.back(-1);
			  </script>";
		die;
	}
}

// Mata a sessão de acordo com os eventos da tela
if ( $_REQUEST["evento"] == 'N' ){
	unset( $_SESSION["academico"]["edpid"] );
}

// Define as variáveis de sessão usadas na tela
$tpetipo     			= $_REQUEST["tpetipo"] ? $_REQUEST["tpetipo"] : $_SESSION["academico"]["tpetipo"];
$edpidhomo   			= $_REQUEST["edpidhomo"] ? $_REQUEST["edpidhomo"] : $_SESSION["academico"]["edpidhomo"];
$prtid 		 			= $_SESSION['academico']['prtid'];
$entidcampus 			= $_SESSION['academico']['entidcampus'];
$edpideditalhomologacao = $_SESSION['academico']['edpideditalhomologacao'] ? $_SESSION['academico']['edpideditalhomologacao'] : $_REQUEST['edpideditalhomologacao'];

// busca os dados
if ( $_SESSION["academico"]["edpid"] || $_REQUEST["edpid"] ){
	
	$edpid = $_REQUEST["edpid"] ? $_REQUEST["edpid"] : $_SESSION["academico"]["edpid"];
	$dados = "";
	$dados = aca_busca_editais( $_SESSION["academico"]["edpid"] );
	
}

// Realiza as açoes da tela
switch ( $_REQUEST["evento"] ){
	case 'cadastra':
		aca_cadastra_editais( $_REQUEST );
	break;
	case 'atualiza':
		aca_atualiza_editais( $_REQUEST );
	break;
	case 'exclui_homologacao':
		aca_exclui_editais_homo( $_REQUEST["edpid"] );
	break;
	case 'exclui_nomeacao':
		aca_exclui_editais_nomeacao( $_REQUEST["edpid"] );
	break;
	default:
	break;
}

// Cabeçalho padrão - SIMEC
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


if ( $_REQUEST['acao'] == 'H' ){
	$titulo = "Edital de Homologação";
	$parametro = array(
						'',
						'&edpid=' . $edpidhomo
					  );
}elseif ( $_REQUEST['acao'] == 'N' ){
	$titulo = "Portaria de Efetivação";
	$parametro = array(
						'',
						'&edpid=' . ($edpideditalhomologacao ? $edpideditalhomologacao : $_REQUEST['edpideditalhomologacao'])
					  );	
}else{
	$titulo = "Edital de Concurso";
}

// Abas do módulo
#$db->cria_aba($abacod_tela, $url, $parametro);
monta_titulo( $titulo, "");	

// mensagem de bloqueio
$bloqueado = academico_mensagem_bloqueio( $_SESSION['academico']['orgid']);

$perfilNotBloq = array(
    PERFIL_IFESCADBOLSAS,
    PERFIL_IFESCADCURSOS,
    PERFIL_IFESCADASTRO,
    PERFIL_MECCADBOLSAS,
    PERFIL_MECCADCURSOS,
    PERFIL_MECCADASTRO,
    PERFIL_ADMINISTRADOR,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_REITOR,
    PERFIL_PROREITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança
$bloqueado = (!$bloqueado && $permissoes['gravar']) ? false : true;

$habil = $bloqueado ? 'N' : $habil;
$habilitado = $bloqueado ? false : $habilitado;

// Define tipo de evento do formulario
$evento = !empty($edpid) ? 'atualiza' : 'cadastra';

// Monta o Cabeçalho
if ( (isset($edpid) || isset($edpidhomo) ) && isset($prtid) && isset($entidcampus) ){ 
	$autoriazacaoconcursos = new autoriazacaoconcursos();
	$dadoedital = $edpid ? $edpid : $edpidhomo;
	$cabecalhoEdital = $autoriazacaoconcursos->cabecalhoedital($dadoedital, $prtid, $entidcampus );
}

?>
<script>

	/**
	 *
	 */
	function submeter( ){
	
		if (document.formulario.edpdtcriacao.value != ""){
			if(!validaData(document.formulario.edpdtcriacao)){
				alert("A data de criação informada é inválida!");
				document.formulario.edpdtcriacao.focus();
				return false;
			}
		}
		<?if( $tpetipo == ACA_TPEDITAL_NOMEACAO){ ?>
		if (document.formulario.prtidautprovimento.value == ""){
			alert("Informe a Portaria de Provimentos !");
			return false;
		}
		<?} ?>		
		if (document.formulario.edpdtpubldiario.value != ""){
			if(!validaData(document.formulario.edpdtpubldiario)){
				alert("A data de DOU informada é inválida!");
				document.formulario.edpdtpubldiario.focus();
				return false;
			}
		}
		
		document.formulario.submit();
		
	}
	
	/**
	 *
	 */
	function inserirEditalHomologacao(){
		location.href = '/academico/academico.php?modulo=principal/cadedital&acao=H&evento=N&edpidhomo=<?=$edpid;?>&tpetipo=<?=ACA_TPEDITAL_HOMOLOGACAO;?>';
	}
	/**
	 *
	 */
	function inserirEditalNomeacao(){
		location.href = '/academico/academico.php?modulo=principal/cadedital&acao=N&evento=N&tpetipo=<?=ACA_TPEDITAL_NOMEACAO;?>&edpideditalhomologacao=<?=$edpid;?>';
	}
	/**
	 *
	 */
	function selecionarProvimento(){
		window.open('?modulo=principal/listarPortariasProvimentos&acao=C','teste','height=600,width=800,status=1,toolbar=0,menubar=no,scrollbars=1,resizable=1');
	}
	/**
	 *
	 */
	function apagarNomeacao(edpid){
		if(confirm('Deseja realmente excluir esta Efetivação?'))
			location.href='/academico/academico.php?modulo=principal/cadedital&acao=C&evento=exclui_nomeacao&edpid='+edpid;
	}
	/**
	 *
	 */
	function apagarHomologacao(edpid){
		if(confirm('Deseja realmente excluir esta Homologação?'))
			location.href='/academico/academico.php?modulo=principal/cadedital&acao=C&evento=exclui_homologacao&edpid='+edpid;
	}
	/**
	 *
	 */
	function abrir_edital(edpid, tpeid){ 
		var acao;
		
		if ( tpeid == 5 )
			acao = 'N';
		else if ( tpeid == 2 )
			acao = 'C';	
		else
			acao = 'H';
		
		location.href="academico.php?modulo=principal/cadedital&acao=" + acao + "&evento=A&edpid=" + edpid + "&tpetipo=" + tpeid;	
	}
	 
//	function abrir_edital(edpid, tpeid){ 
//		location.href="academico.php?modulo=principal/cadedital&acao=C&evento=A&edpid=" + edpid + "&tpetipo=" + tpeid;	
//	}
	
	/**
	 *
	 */
	function apagarHomologacao(edpid){
		if(confirm('Deseja realmente excluir esta Homologação?'))
			location.href='/academico/academico.php?modulo=principal/cadedital&acao=C&evento=exclui_homologacao&edpid='+edpid;
	}
	
</script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/superTitle.css"/>
<script type="text/javascript" src="../includes/remedial.js"></script>
<script type="text/javascript" src="../includes/superTitle.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/listagem2.css">
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
<? echo $cabecalhoEdital; ?>
<form method="POST"  name="formulario" enctype="multipart/form-data">
	<input type="hidden" name="edpid" id="edpid" value="<?=$edpid?>">
	<input type="hidden" name="evento" id="evento" value="<?php echo $evento;?>">
	<input type="hidden" name="prtid" id="prtid" value="<?php echo $_SESSION["academico"]["prtid"]; ?>">
	<input type="hidden" name="usucpf" id="usucpf" value="<?php echo $_SESSION["usucpf"];?>">
	<input type="hidden" name="entidcampus" id="entidcampus" value="<?php echo $_SESSION["academico"]["entidcampus"];?>">
	<input type="hidden" name="entid" id="entid" value="<?php echo $_SESSION["academico"]["entid"];?>">
	<input type="hidden" name="edpidhomo" id="edpidhomo" value="<?php echo $_REQUEST["edpidhomo"]; ?>">
	<input type="hidden" name="edpideditalhomologacao" id="edpideditalhomologacao" value="<?php echo $_REQUEST["edpideditalhomologacao"]; ?>">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:20%">Tipo:</td>
			<td>
				<?php

					//Define o tipo de edital a ser cadastrado/atualizado				
					switch ( $tpetipo ){
						case ACA_TPEDITAL_HOMOLOGACAO:
							echo "<b> Homologação de Concurso</b> <input type=hidden name=tpeid id=tpeid value=".ACA_TPEDITAL_HOMOLOGACAO.">";
						break;
						case ACA_TPEDITAL_NOMEACAO:
							echo "<b> Portaria de Efetivação </b><input type=hidden name=tpeid id=tpeid value=".ACA_TPEDITAL_NOMEACAO.">";
						break;
						default:
							echo "<b> Edital de Concurso </b><input type=hidden name=tpeid id=tpeid value=".ACA_TPEDITAL_PUBLICACAO.">";
						break;
					}
					
				?>
			</td>
		</tr>
		<tr>
			<td align='right' width="30%" class="SubTituloDireita">Número de Controle<?=$texto?>:</td>
		    <td>
		    	<?php 
		    		$n_controle = !empty($edpid) ? $edpid : 'Não Informado';
		    		echo $n_controle;
		    	?>
		    </td>
		</tr>
	<? if ($tpetipo == ACA_TPEDITAL_NOMEACAO) {
		if ($dados["prtidautprovimento"]){
			$sql="SELECT prtnumero
					FROM academico.portarias
					WHERE prtid =".$dados["prtidautprovimento"];
			$numportariaprovimento=$db->pegaUm($sql); 
		}
		?>
		<tr>
			<td align='right' width="30%" class="SubTituloDireita">Portaria de Autorização de Provimentos:</td>
		    <td valign="middle">
				<div style="float:left;position:relative;padding-top:3px"  id=DadosProvimento>Nº de Controle: <?=($dados["prtidautprovimento"]) ? $numportariaprovimento : "Não informado";?></div>
				<input style="float:left;position:relative;margin-left:10px" type="button" class="botao" name="btassociar" value="Pesquisar" onclick="selecionarProvimento();">
				<img style="float:left;position:relative;padding-top:3px" src="../imagens/obrig.gif" title="Indica campo obrigatório." border="0">
				<input type="hidden" name="prtidautprovimento" id="prtidautprovimento" value="<?=$dados["prtidautprovimento"];?>">
		    </td>
		</tr>
	
	<? }?>
		<tr>
			<td align='right' width="30%" class="SubTituloDireita">Número:</td>
		    <td>
		    	<?php
		    		$edpnumero = $dados["edpnumero"];
		    		echo campo_texto('edpnumero','N',$habil,'',15,10,'','');
		    	?>
		    </td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita">Data:</td>
		    <td>
		    	<?php
		    		$edpdtcriacao = formata_data($dados["edpdtcriacao"]);
		    		echo campo_data('edpdtcriacao','N',$habil,'','','','');
		    	?>
		    </td>
		<tr bgcolor="#cccccc">
		   <td colspan="2" align="center"><b>Publicação no DOU</b></td>	  	  
		</tr>
		<tr>
		    <td align='right' class="SubTituloDireita">Número do DOU:</td>
		    <td>
		    	<?php
		    		$edpnumdiario = $dados["edpnumdiario"];
		    		echo campo_texto('edpnumdiario','N',$habil,'',15,10,'','');
		    	?>
		    </td>
		</tr> 
		<tr>
		    <td align='right' class="SubTituloDireita">Número da Seção:</td>
		    <td>
		    	<?php
		    		$edpsecaodiario = $dados["edpsecaodiario"];
		    		echo campo_texto('edpsecaodiario','N',$habil,'',15,10,'','');
		    	?>
		    </td>
		</tr> 
		<tr>
			<td align='right' class="SubTituloDireita">Data do DOU:</td>
		    <td>
		    	<?php
		    		$edpdtpubldiario = formata_data($dados["edpdtpubldiario"]);
		    		echo campo_data('edpdtpubldiario','N',$habil,'','','','');
		    	?>
		    </td>
		</tr>
		<tr>
		    <td align='right' class="SubTituloDireita">Número da Página:</td>
		    <td>
		    	<?php
		    		$edpdiariopagina = $dados["edpdiariopagina"];
		    	 	echo campo_texto('edpdiariopagina','N',$habil,'',15,10,'','');
		    	?>
		    </td>
		</tr> 
		<?
		if($habilitado){
			echo("<tr style='background-color: #cccccc'>
					<td align='right' style='vertical-align:top; width:25%'></td>
					<td>
						<input type='button' class='botao' name='btassociar' value='Gravar' onclick='submeter();'>
					</td>
				</tr>");
		}
		?>		
	</table>	
</form>

<!--  Lista os editais de homologação  -->
<?php if( $edpid ){ ?>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td>
			<?php
	
				if ( $habilitado ){
					$btexcluir = "<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"javascript: apagarHomologacao(' ||edp.edpid || ');\" title=\"Excluir\">";
				}else{
					$btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
				}
				
				if( $tpetipo == ACA_TPEDITAL_PUBLICACAO){
					echo "<br> <b>Edital de Homologação Concurso<br><br>";
					$inserir_homologacao = true;
					// Mostra o Edital de Homologação
					$sql = "
				        SELECT
							'<center> <img  style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abrir_edital('|| edp.edpid ||', 3)\" src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\">&nbsp {$btexcluir}</center>' as acao, 
							'<center>' ||TO_CHAR(edp.edpdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
							'<center>' || edp.edpid ||'</center>' AS cod,
							'<center>' || edp.edpnumero ||'</center>' as numero,
							'<center>' || u.usunome ||'</center>' as nome,
							'<div onmousemove=\"SuperTitleAjax(\'quadro_niveis.php?edpid=' || edp.edpid || '&tpeid=" . ACA_TPEDITAL_HOMOLOGACAO . "  \', this );\"
										  onmouseout=\"SuperTitleOff( this );\"
										  style=\"width:100%; color:#0066CC;  text-align:center;\">
									      ' || COALESCE(SUM(lep.lepvlrhomologado), 0) || '
							</div>' AS homologado
							--COALESCE(SUM(lep.lepvlrhomologado), 0) AS homologado
						FROM academico.editalportaria AS edp
							INNER JOIN academico.tipoeditalportaria tp ON tp.tpeid= edp.tpeid
							INNER JOIN seguranca.usuario u ON u.usucpf = edp.usucpf
							LEFT JOIN academico.lancamentoeditalportaria lep ON lep.edpid = edp.edpid AND
						     													lep.lepstatus = 'A'
						WHERE edp.edpstatus = 'A' AND
							edp.edpidhomo = ".$edpid."
						GROUP BY
							acao,
							dtinclusao,
							edp.edpid,
							numero,
							nome	 
						ORDER BY 
							edp.edpid DESC";

	
					$cabecalho = array( "Ação", 
										"Data Inclusão",
										"Nº de Controle",
										"Nº Edital",
										"Nome",
										"Total de Homologado");
//					monta_lista($sql,$cabecalho="",$perpage,$pages,$soma,$alinha,$valormonetario="S",$nomeformulario="",$celWidth="",$celAlign="")
				$db->monta_lista( $sql, $cabecalho, 15, 10, 'N','','' );	
				}
				
				if( $tpetipo == ACA_TPEDITAL_HOMOLOGACAO ){
					if ( $habilitado ){
						$btexcluir = "<img style=\"cursor: pointer;\" src=\"/imagens/excluir.gif \" border=0 onclick=\"javascript: apagarNomeacao(' ||edp.edpid || ');\" title=\"Excluir\">";
					}else{
						$btexcluir = "<img src=\"/imagens/excluir_01.gif \" border=0 title=\"Excluir\">";
					}
					if( !empty($edpidhomo) and $edpidhomo > 0){
						
//						echo "<br> <b>Edital de Concurso<br><br>";
//						// Mostra o Editail de Publicação
//						$sql = " SELECT
//									'<center> <img  style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abrir_edital('|| edp.edpid ||', 2)\" src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\"></center>' as acao, 
//									'<center>' ||TO_CHAR(edp.edpdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
//									'<center>' || edp.edpid ||'</center>' AS cod,
//									'<center>' || edp.edpnumero ||'</center>' as numero,
//									'<center>' || u.usunome ||'</center>' as nome
//								FROM academico.editalportaria AS edp
//									INNER JOIN academico.tipoeditalportaria tp ON tp.tpeid= edp.tpeid
//									INNER JOIN seguranca.usuario u ON u.usucpf = edp.usucpf
//								WHERE  edp.edpstatus = 'A' AND
//									edp.edpid = ".$edpidhomo." ORDER BY edp.edpid DESC";
//								
//						$cabecalho = array(	"Ação", 
//											"Data Inclusão",
//											"Nº de Controle",
//											"Nº Edital",
//											"Nome");
//						$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );
					
						echo "<br> <b>Portarias de Efetivação<br><br>";
						
						// Mostra as Portarias de Nomeação
						$sql = " SELECT
									'<center> <img  style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abrir_edital('|| edp.edpid ||', 5)\" src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\">&nbsp {$btexcluir}</center>' as acao, 
									'<center>' ||TO_CHAR(edp.edpdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
									'<center>' || edp.edpid ||'</center>' AS cod,
									'<center>' || edp.edpnumero ||'</center>' as numero,
									'<center>' || u.usunome ||'</center>' as nome,
									'<div onmousemove=\"SuperTitleAjax(\'quadro_niveis.php?edpid=' || edp.edpid || '&tpeid=" . ACA_TPEDITAL_NOMEACAO . "  \', this );\"
										  onmouseout=\"SuperTitleOff( this );\"
										  style=\"width:100%; color:#0066CC;  text-align:center;\">
									      ' || COALESCE(SUM(lep.lepvlrprovefetivados), 0) || '
									 </div>' AS efetivado
									-- COALESCE(SUM(lep.lepvlrprovefetivados), 0) AS efetivado
								FROM academico.editalportaria AS edp
									INNER JOIN academico.tipoeditalportaria tp ON tp.tpeid= edp.tpeid
									INNER JOIN seguranca.usuario u ON u.usucpf = edp.usucpf
									LEFT JOIN academico.lancamentoeditalportaria lep ON lep.edpid = edp.edpid AND
						    															lep.lepstatus = 'A'	
								WHERE  
									edp.edpstatus = 'A' AND
									edp.edpideditalhomologacao = ".$edpid." 
								GROUP BY
									acao,
									dtinclusao,
									edp.edpid,
									numero,
									nome									
								ORDER BY 
									edp.edpid DESC";
								
						$cabecalho = array(	"Ação", 
											"Data Inclusão",
											"Nº de Controle",
											"Nº Portaria",
											"Nome",
											"Total de Efetivado");
						$db->monta_lista( $sql, $cabecalho, 15, 10, 'N','','' );
					}
				}
//				if( $tpetipo == ACA_TPEDITAL_NOMEACAO and isset($edpideditalhomologacao)){
//					$inserir_homologacao = true;
//					echo "<br> <b>Edital de Homologação<br><br>";
//					// Mostra o Edital de Homologação
//					$sql = "
//				        SELECT
//							'<center> <img  style=\"padding-right: 5px; cursor: pointer;\" onclick=\"abrir_edital('|| edp.edpid ||', 3)\" src=\"/imagens/alterar.gif \" border=0 alt=\"Ir\">&nbsp</center>' as acao, 
//							'<center>' ||TO_CHAR(edp.edpdtinclusao,'DD/MM/YYYY') ||'</center>' as dtinclusao,
//							'<center>' || edp.edpid ||'</center>' AS cod,
//							'<center>' || edp.edpnumero ||'</center>' as numero,
//							'<center>' || u.usunome ||'</center>' as nome
//						FROM academico.editalportaria AS edp
//							INNER JOIN academico.tipoeditalportaria tp ON tp.tpeid= edp.tpeid
//							INNER JOIN seguranca.usuario u ON u.usucpf = edp.usucpf
//						WHERE edp.edpstatus = 'A' AND
//							edp.edpid = ".$edpideditalhomologacao." ORDER BY edp.edpid DESC";
//				
//					$cabecalho = array( "Ação", 
//										"Data Inclusão",
//										"Nº de Controle",
//										"Nº Edital",
//										"Nome");
//					
//				$db->monta_lista_simples( $sql, $cabecalho, 50, 10, 'N', '', '' );	
//				}
				
				//if ( $db->pegaLinha($sql) ){
					//$inserir_homologacao = false;
				//}
				if ($tpetipo == ACA_TPEDITAL_HOMOLOGACAO )
					$inserir_nomeacao = true;		
			?>
		</td>
	</tr>
	<tr>
		<td>
			<?php if ($inserir_homologacao && $habilitado){?>
				<a href="#" onclick="javascript: inserirEditalHomologacao();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">
				&nbsp;&nbsp;Inserir Novo Edital de Homologação de Concurso</a>
			<?php } ?>
		</td>
	</tr>
	<tr>
		<td>
			<?php if ($inserir_nomeacao && $habilitado){?>
				<a href="#" onclick="javascript: inserirEditalNomeacao();"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir">
				&nbsp;&nbsp;Inserir Nova Portaria de Efetivação</a>
			<?php } ?>
		</td>
	</tr>
</table>
<?} ?>
</div>