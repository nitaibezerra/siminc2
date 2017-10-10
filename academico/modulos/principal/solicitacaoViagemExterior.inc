<?php
include APPRAIZ ."includes/workflow.php";
$_SESSION['academico']['entid'] = $_REQUEST['entidunidade'] ? $_REQUEST['entidunidade'] : $_SESSION['academico']['entid'];
if($_REQUEST['PopUpMinuta']){
	$arrDados = carregarProgramaViagemExterior();
	if($arrDados){
		extract($arrDados);
		if($entid){
			$sql = "select
						ent.entnome,
						ent.entnumcomercial,
						ent.entunicod,
						ende.endcep,
						ende.endlog,
						ende.endbai,
						ende.estuf,
						mun.mundescricao
					from 
						entidade.entidade ent
					left join
						entidade.endereco ende ON ende.entid = ent.entid
					left join
						territorios.municipio mun ON mun.muncod = ende.muncod
					where 
						ent.entid = $entid";
			$arrEnt = $db->pegaLinha($sql);
			if($arrEnt){
				extract($arrEnt);
			}
		}
		if($docid){
			$sql = "select
						to_char(hd.htddata,'DD/MM/YYYY') as htddata
					from 
						workflow.historicodocumento hd
					where
						hd.docid = $docid
					order by
						hd.htddata desc
					limit 1";
			$dataDocid = $db->pegaUm($sql);
		}
	}
	ob_clean();
	?>
	<style>
		.cabecalho{text-align:center;font-weight:bold;font-size:18px;}
		table.tbl {border-collapse: collapse;}
		table.tbl tr td {border:1px solid black;}
		table.tbl2 {border-top:0px;}
		.texto{text-align:left;}
		table.tbl_not {border:0px;}
		*{font-family:Arial, Helvetica, sans-serif;font-size:12px;}
		.bold{font-weight:bold}
		@media print {.notprint { display: none }}
		@media screen {.notscreen { display: none }}
	</style>
	<div class="notprint" style="text-align:right;width:100%" >
	<a href="javascript:print()">Imprimir</a>
	</div>
	<div class="cabecalho" >	
		<img width="150px" height="150px" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/brasao.gif" /><br /><br />
		<span style="font-size:14px">MINISTÉRIO DA EDUCAÇÃO</span><br /><br />
		Autorização para Diárias, Passagens e Locomoção para Viagens para Exterior.<br /><br />
	</div>
	<table width="100%" style="border-bottom:0px" class="tbl" cellSpacing="0" cellPadding="3" align="center" >
	  <tr>
	    <td class="bold" >Nome da Unidade:</td>
	    <td colspan="5" ><?php echo $entnome ?></td>
	  </tr>
	  <tr>
	    <td class="bold" >Código da Unidade Orçamentária:</td>
	    <td width="45%" ><?php echo $entunicod ? $entunicod : "N/A" ?></td>
	    <td class="bold" width="10%" >Telefone:</td>
	    <td colspan="3"  width="10%" ><?php echo $entnumcomercial ? $entnumcomercial : "N/A" ?></td>
	  </tr>
	  <tr>
	    <td class="bold" >Endereço:</td>
	    <td colspan="5" ><?php echo $endlog ? $endlog.($endbai ? " - ".$endbai : "") : "N/A" ?></td>
	  </tr>
	 </table>
	 <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
	   <tr>
	    <td class="bold" width="10%" >CEP:</td>
	    <td width="10%" ><?php echo $endcep ? $endcep : "N/A" ?></td>
	    <td class="bold" width="10%" >Cidade:</td>
	    <td ><?php echo $mundescricao ? $mundescricao : "N/A" ?></td>
	    <td  class="bold" width="10%" >UF:</td>
	    <td  width="10%" ><?php echo $estuf ? $estuf : "N/A" ?></td>
	  </tr>
	</table>
	
	<?
	if($avecpfresp != ''){ ?>
		
		<?php $data = explode("/",$dataDocid);
			$diaDocid = $data[0];
			$mesDocid =  strtolower(retornaMesPorExtenso($data[1]));
			$anoDocid = $data[2]; ?>
			
		<table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
		<tr>
		    <td>
		    <div class="texto">
		    	<br />
		    	À Sua Excelência o Senhor<br />
				LUIZ CLÁUDIO COSTA<br />
				Secretário Executivo do Ministério da Educação<br /><br />
				<span style="margin-left:50px">Senhor Secretário Executivo,</span><br />
				<p><span style="margin-left:50px">Solicito</span> autorização para a concessão de diárias e passagens para viagens ao exterior no âmbito desta Instituição Federal de Ensino para o exercício de <?=$anoDocid?>, nos termos do Decreto nº 7.689, de 2 de março de 2012:</p>
			</div>
			<?php $arrProgramas = recuperaProgramaViagemExterior() ?>
			<?php $arrQtdeParc = $_REQUEST['aveid'] ? recuperaProgramaViagemExteriorPorAveid($_REQUEST['aveid']) : array() ?>
			<?php if($arrProgramas): ?>
				<table class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
				<tr>
					<td align="center" ><b>Programa</b></td>
					<td align="center" ><b>Número de Participantes</b></td>
				</tr>
				<?php foreach($arrProgramas as $prog): ?>
					<?php if($arrQtdeParc[$prog['pveid']]): ?>
						<tr>
							<td align="center" ><?php echo $prog['pvedsc'] ?></td>
							<td align="center" >
								<?php echo $arrQtdeParc[$prog['pveid']];$total+=$arrQtdeParc[$prog['pveid']];?>
							</td>
						</tr>
					<?php endif; ?>
				<?php endforeach; ?>
				<tr>
					<td align="center" ><b>Total</b></td>
					<td align="center" >
						<b><?php echo $total ?></b>
					</td>
				</tr>
				</table>
			<?php endif; ?>
			<br />
			<table border=0 class="tbl_not" width="100%" cellSpacing="0" cellPadding="3" align="center" >
				<tr>
					<td width="50%" align="center" >
						<?php echo $mundescricao ? $mundescricao : "N/A" ?> - <?php echo $dataDocid ?><br />
						Local e Data
					</td>
					<td align="center" >
						Reitor: <?php echo $avenomereitor ?><br />
						CPF: <?php echo $avecpfreitor ?><br />
						Matrícula SIAPE: <?php echo $avematsiapereitor ?>
					</td>
				</tr>
			</table>
			<br />
			
			<b>Autorização nº <?php echo $avenumauto ?> </b>, de <?php echo $diaDocid ?> de <?php echo $mesDocid ?> de <?php echo $anoDocid ?>.<br /><br />
			<?if($anoDocid > 2012){?>
				<span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar a legislação vigente e suas alterações.<br /><br />
			<?}else{?>
				<span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar os limites constantes do Anexo I, da Portaria MEC nº 362, de 10 de abril de 2012, e suas alterações.<br /><br />
			<?}?>
			<center>
			<img width="232" height="104" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_secretario.png" /><br />
			<!-- 
			<b><span style="font-size:14px">LUIZ CLÁUDIO COSTA</span></b><br /><br />
			<b>Secretário Executivo do Ministério da Educação</b>
			 -->
			</center>
		    </td>
		  </tr>
		 </table>
		 
	<?}else{?>
	
		<?php $data = explode("/",$dataDocid);
			$diaDocid = $data[0];
			$mesDocid =  strtolower(retornaMesPorExtenso($data[1]));
			$anoDocid = $data[2]; ?>
			
        <table width="100%" class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center">
		<tr>
		    <td>
		    <div class="texto">
		    	<br />
		    	À Sua Excelência o Senhor<br />
				CID FERREIRA GOMES<br />
				Minístro de Estado da Educação<br /><br />
				<span style="margin-left:50px">Senhor Ministro,</span><br />
				<p><span style="margin-left:50px">Solicito</span> autorização para a concessão de diárias, passagens e locomoção para viagens ao exterior no âmbito desta Instituição Federal de Ensino para o exercício de <?=$anoDocid?>, nos termos do artigo 4º, § 1º da Portaria MEC nº 446, de 20 de abril de 2011:</p>
			</div>
			<?php $arrProgramas = recuperaProgramaViagemExterior() ?>
			<?php $arrQtdeParc = $_REQUEST['aveid'] ? recuperaProgramaViagemExteriorPorAveid($_REQUEST['aveid']) : array() ?>
			<?php if($arrProgramas): ?>
				<table class="tbl tbl2" cellSpacing="0" cellPadding="3" align="center" >
				<tr>
					<td align="center" ><b>Programa</b></td>
					<td align="center" ><b>Número de Participantes</b></td>
				</tr>
				<?php foreach($arrProgramas as $prog): ?>
					<?php if($arrQtdeParc[$prog['pveid']]): ?>
						<tr>
							<td align="center" ><?php echo $prog['pvedsc'] ?></td>
							<td align="center" >
								<?php echo $arrQtdeParc[$prog['pveid']];$total+=$arrQtdeParc[$prog['pveid']];?>
							</td>
						</tr>
					<?php endif; ?>
				<?php endforeach; ?>
				<tr>
					<td align="center" ><b>Total</b></td>
					<td align="center" >
						<b><?php echo $total ?></b>
					</td>
				</tr>
				</table>
			<?php endif; ?>
			<br />
			<table border=0 class="tbl_not" width="100%" cellSpacing="0" cellPadding="3" align="center" >
				<tr>
					<td width="50%" align="center" >
						<?php echo $mundescricao ? $mundescricao : "N/A" ?> - <?php echo $dataDocid ?><br />
						Local e Data
					</td>
					<td align="center" >
						Reitor: <?php echo $avenomereitor ?><br />
						CPF: <?php echo $avecpfreitor ?><br />
						Matrícula SIAPE: <?php echo $avematsiapereitor ?>
					</td>
				</tr>
			</table>
			<br />
			
			<b>Autorização nº <?php echo $avenumauto ?> </b>, de <?php echo $diaDocid ?> de <?php echo $mesDocid ?> de <?php echo $anoDocid ?>.<br /><br />
			<span style="margin-left:50px">Autorizado</span>, devendo essa Instituição observar os limites constantes do Anexo I, da Portaria MEC nº 446, de 20 de abril de 2011, e suas alterações.<br /><br />
			<center>
			<img width="122" height="44" src="http://<?php echo $_SERVER['HTTP_HOST'] ?>/imagens/assinaturas_dirigentes/assinatura_ministro.png" /><br />
			<b><span style="font-size:14px">CID FERREIRA GOMES</span></b><br /><br />
			<b>Ministro de Estado da Educação</b>
			</center>
		    </td>
		  </tr>
		 </table>	
		 
	<?}?>
	
	
	<?php
	$html = ob_get_contents();
	ob_clean();
	//echo $html;
	
	$content = http_build_query( array('conteudoHtml'=> utf8_encode($html) ) );
	$context = stream_context_create(array( 'http'    => array(
							                'method'  => 'POST',
							                'content' => $content ) ));

	$contents = file_get_contents('http://ws.mec.gov.br/ws-server/htmlParaPdf', null, $context );
	header('Content-Type: application/pdf');
	header("Content-Disposition: attachment; filename='autorizacao_".str_replace(".","_",$avenumauto)."'");
	echo $contents;
	exit;
}
function retornaMesPorExtenso($num)
{
	switch($num) {
		case "01": $mes = "Janeiro";   break;
		case "02": $mes = "Fevereiro"; break;
		case "03": $mes = "Março";     break;
		case "04": $mes = "Abril";     break;
		case "05": $mes = "Maio";      break;
		case "06": $mes = "Junho";     break;
		case "07": $mes = "Julho";     break;
		case "08": $mes = "Agosto";    break;
		case "09": $mes = "Setembro";  break;
		case "10": $mes = "Outubro";   break;
		case "11": $mes = "Novembro";  break;
		case "12": $mes = "Dezembro";  break;
	}
	return $mes;
}

function recuperaProgramaViagemExterior()
{
	global $db;
	$sql = "select * from academico.progviagemexterior where pvestatus = 'A' order by pvedsc";
	return $db->carregar($sql);
}

function recuperaProgramaViagemExteriorPorAveid($aveid)
{
	global $db;
	$sql = "select
				pveid,
				qtde
			from 
				academico.autvexqtdeprg
			where 
				aveid = $aveid ";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		foreach($arrDados as $dado){
			$arrQtde[$dado['pveid']] = $dado['qtde'];
		}
		return $arrQtde;
	}else{
		return array();
	}

}

function carregarProgramaViagemExterior()
{
	global $db;
	$aveid = $_REQUEST['aveid'];
	$sql = "select * from academico.autviagemexterior where avestatus = 'A' and aveid = $aveid";
	return $db->pegaLinha($sql);
}

function excluirAutorizacao()
{
	global $db;
	$aveid = $_POST['aveid'];
	$sql = "update academico.autviagemexterior set avestatus = 'I' where aveid = $aveid";
	$db->executar($sql);
	$db->commit($sql);
	$_SESSION['academico']['msg'] = "Operação Realizada com sucesso!";
	header("Location: academico.php?modulo=principal/solicitacaoViagemExterior&acao=A");
	exit;
}

function listaAutorizacaoViagem()
{
	global $db;
	$entid = $_SESSION['academico']['entid'];

	if(!$entid){
		return false;
	}
	
	$sql = "select
				(CASE WHEN ( select ed.esdid from workflow.documento d inner join workflow.estadodocumento ed on ed.esdid = d.esdid where d.docid = tbl1.docid) = ".WF_SOLICITACAO_VIAGEM_EM_CADASTRAMENTO." 
					THEN 
					'<div style=\"white-space: nowrap\" >
						<img src=\"../imagens/alterar.gif\" alt=\"Alterar\"  title=\"Alterar\" class=\"link\" onclick=\"editarAutorizacao(' || aveid || ')\" /> 
						<img src=\"../imagens/excluir.gif\" alt=\"Excluir\"  onclick=\"excluirAutorizacao(' || aveid || ')\" title=\"Excluir\" class=\"link\" />
					</div>'
					ELSE
						(CASE WHEN avenumauto IS NOT NULL
							THEN
								'<div style=\"white-space: nowrap\" >
									<img src=\"../imagens/alterar.gif\" alt=\"Alterar\"  title=\"Alterar\" class=\"link img_middle\" onclick=\"editarAutorizacao(' || aveid || ')\" />
									<img src=\"../imagens/print.gif\" alt=\"Visualizar Impressão\" class=\"link img_middle\"  title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta(' || aveid || ')\" /> 
								</div>'
							ELSE
								'<div style=\"white-space: nowrap\" >
									<img src=\"../imagens/alterar.gif\" alt=\"Alterar\"  title=\"Alterar\" class=\"link\" onclick=\"editarAutorizacao(' || aveid || ')\" /> 
								</div>'
						END) 
				END) as acao,
				aveid,
				'<a href=\"javascript:editarAutorizacao(' || aveid || ')\"  >' || avenomereitor || '</a>' as avenomereitor,
				COALESCE('<img src=\"../imagens/print.gif\" alt=\"Visualizar Impressão\" class=\"link img_middle\"  title=\"Visualizar Impressão\" class=\"link\" onclick=\"exibeMinuta(' || aveid || ')\" /> <a href=\"javascript:exibeMinuta(' || aveid || ')\"  >' || avenumauto || '</a>','N/A') as avenumauto,
				(select
					ed.esddsc
				from 
					workflow.documento d
				inner join 
					workflow.estadodocumento ed on ed.esdid = d.esdid
				where
					d.docid = tbl1.docid) 
				as docid,
				( select sum(qtde) from academico.autvexqtdeprg atp where atp.aveid = tbl1.aveid) as qtde,
				'<img src=\"../imagens/fluxodoc.gif\" class=\"link img_middle\" onclick=\"wf_exibirHistorico(\'' || docid || '\')\"  />' || to_char(avedata,'DD/MM/YYYY') as avedata
			from 
				academico.autviagemexterior tbl1
			where 
				avestatus = 'A'
			and
				entid = $entid";
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	foreach($arrDados as $chave => $dado){
		$sql = "select 
					pvedsc || ' - ' || qtde as pvedsc
				from
					academico.autvexqtdeprg atp
				inner join
					academico.progviagemexterior prg ON prg.pveid = atp.pveid 
				where 
					atp.aveid = {$dado['aveid']}
				order by
					pvedsc";
		$arrProg = $db->carregarColuna($sql);
		$arrDados[$chave]['programa'] = ($arrProg ? implode("<br />",$arrProg) : "N/A");
		$arrDados[$chave]['qtde_part'] = $arrDados[$chave]['qtde'];
		$arrDados[$chave]['estado'] = $arrDados[$chave]['docid'];
		$arrDados[$chave]['data'] = $arrDados[$chave]['avedata'];
		unset($arrDados[$chave]['avedata']);
		unset($arrDados[$chave]['qtde']);
		unset($arrDados[$chave]['docid']);
	}
	$cabecalho = array("Ação","Num. Solicitação","Reitor","Número de Autorização","Programas","Número de Participantes","Situação","Data da Solicitação");
	$db->monta_lista_array($arrDados,$cabecalho,50,10,'N','center');
}

function salvarAutorizacao()
{
	global $db;
	extract($_POST);
	
	$entid = $_SESSION['academico']['entid'];
	
	if(!$entid){
		$_SESSION['academico']['msg'] = "Informações do Reitor não encontradas!";
		return false;
	}
	
	$sql = "SELECT
				usu.usunome as avenomereitor,
				usu.usucpf as avecpfreitor,
				nu_matricula_siape as avematsiapereitor
			FROM
				seguranca.usuario usu
			INNER JOIN
				academico.usuarioresponsabilidade ur ON ur.usucpf = usu.usucpf
			INNER JOIN
				seguranca.usuario_sistema us ON us.usucpf = ur.usucpf
			LEFT  JOIN
			 	siape.tb_siape_cadastro_servidor siape ON siape.nu_cpf::text = usu.usucpf::text
			WHERE
				ur.rpustatus = 'A'
				AND us.suscod = 'A'
				AND	ur.pflcod = ".PERFIL_REITOR."
				AND us.sisid = ". $_SESSION['sisid'] ."
				AND usu.usunome IS NOT NULL 
				AND usu.usucpf IS NOT NULL 
				AND nu_matricula_siape IS NOT NULL
				AND ur.entid = $entid;";

	$arrDadosReitor = $db->pegaLinha($sql);

	if($arrDadosReitor){
		extract($arrDadosReitor);
	}

	if(!$avenomereitor || !$avecpfreitor || !$avematsiapereitor || !$entid){
		$_SESSION['academico']['msg'] = "Informações do Reitor não encontradas!";
		return false;
	}
	
	if($aveid){
		$sql ="update
					academico.autviagemexterior
			 	set
			 		avenomereitor = '$avenomereitor',
			 		avecpfreitor = '$avecpfreitor',
			 		avematsiapereitor = '$avematsiapereitor'
			 	where
			 		aveid = $aveid
			 	and
			 		entid = $entid";
		$db->executar($sql);
	}else{
		
		$docid = wf_cadastrarDocumento( WF_SOLICITACAO_VIAGEM, "Solicitação Viagem Exterior");
		
		$sql ="insert into 
							academico.autviagemexterior
						(entid,avenomereitor,avecpfreitor,avematsiapereitor,avestatus,docid)
							values
						($entid,'$avenomereitor','$avecpfreitor','$avematsiapereitor','A',$docid) returning aveid";
		$aveid = $db->pegaUm($sql);
	}
	if($aveid){
		$db->executar("delete from academico.autvexqtdeprg where aveid = $aveid");
	}
	if($qtde_part && $aveid){
		foreach($qtde_part as $prg => $qtde){
			if($qtde){
				$sqlI .="insert into 
							academico.autvexqtdeprg
						(pveid,aveid,qtde)
							values
						($prg,$aveid,$qtde);";
			}
		}
		if($sqlI){
			$db->executar($sqlI);
		}
	}
	$_SESSION['academico']['msg'] = "Operação Realizada com sucesso!";
	$db->commit();
	header("Location: academico.php?modulo=principal/solicitacaoViagemExterior&acao=A&requisicao=carregarProgramaViagemExterior&aveid=$aveid");
	exit;
}

if($_REQUEST['requisicaoAjax']){
	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']();
	exit;
}

$_SESSION['academico']['entid'] = $_REQUEST['entid'] ? $_REQUEST['entid'] : $_SESSION['academico']['entid'];
$_SESSION['academico']['entidadenivel'] = 'unidade';

$perfilNotBloq = array(
					   PERFIL_CADASTROGERAL, 
					   PERFIL_MECCADBOLSAS,
					   PERFIL_MECCADCURSOS,
					   PERFIL_MECCADASTRO,
					   PERFIL_ADMINISTRADOR,
					   PERFIL_ALTA_GESTAO,
					   PERFIL_ASSESSORIA_ALTA_GESTAO,
    PERFIL_REITOR,
    PERFIL_INTERLOCUTOR_INSTITUTO,
    PERFIL_PROREITOR);
// Verificando a segurança
$permissoes = verificaPerfilAcademico($perfilNotBloq);
validaAcessoTipoEnsino($permissoes['vertipoensino'],$_SESSION['academico']['orgid']);
// Fim segurança


if($_REQUEST['requisicao']) {
	if($_REQUEST['requisicao'] != "cadastrarAutorizacao"){
		$arrDados = $_REQUEST['requisicao']($_REQUEST);
		if(is_array($arrDados)){
			extract($arrDados);
		}
	}
}

if(!$_SESSION['academico']['entid']) {
	
	die("<script>
			alert('Entidade não encontrada. Navegue novamente.');
			window.location='academico.php?modulo=inicio&acao=C';
		 </script>");
	
}

require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';

#$db->cria_aba($abacod_tela,$url,$parametros);
#academico_monta_cabecalho_sig($_SESSION['academico']['entid'], 'Solicitação Viagem Exterior');
monta_titulo( "Solicitação Viagem Exterior", "");
$autoriazacaoconcursos = new autoriazacaoconcursos();
?>
<style>
	.SubtituloTabela{background-color:#cccccc}
	.negrito{font-weight:bold}
	.bold{font-weight:bold}
	.normal{font-weight:normal}
	.center{text-align: center;}
	.direita{text-align: right;}
	.esquerda{text-align: left;}
	.msg_erro{color:#990000}
	.link{cursor: pointer}
	.mini{width:12px;height:12px}
	.sucess_msg{color: blue;}
	.img_middle{vertical-align:middle;border:0px}
	.hidden{display:none}
	.absolute{position:absolute;padding-top:5px;padding-bottom:5px;padding-left:5px;margin-top:5px;margin-left:50px;border:solid 1px black;background-color: #FFFFFF}
	.fechar{position:relative;right:-5px;top:-26px;}
	.img{background-color:#FFFFFF}
	.red{color:#990000}
</style>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
$(function() {
	 $('[name=btn_salvar]').click(function() {
	 	$("[name=btn_salvar]").val("Carregando...");
	 	$("[name=btn_salvar]").attr("disabled","disabled");
	 	$("#formulario").submit();
	 });
	 $('[name=btn_cancelar]').click(function() {
	 	window.location.href = "academico.php?modulo=principal/solicitacaoViagemExterior&acao=A";
	 });
	<?php if($_SESSION['academico']['msg']): ?>
		alert('<?php echo $_SESSION['academico']['msg'] ?>');
		<?php unset($_SESSION['academico']['msg']); ?>
	<?php endif; ?>
});

function editarAutorizacao(aveid)
{
	var url = "academico.php?modulo=principal/solicitacaoViagemExterior&acao=A&requisicao=carregarProgramaViagemExterior&aveid=" + aveid;
	window.location.href = url;
}
function cadastrarAutorizacao()
{
	$("[name=requisicao]").val("cadastrarAutorizacao");
	$("[name=aveid]").val("");
	$("#formulario").submit();
}
function excluirAutorizacao(aveid)
{
	if(confirm("Deseja realmente excluir?")){
		$("[name=requisicao]").val("excluirAutorizacao");
		$("[name=aveid]").val(aveid);
		$("#formulario").submit();
	}
}
function exibeMinuta(aveid)
{
	window.location.href = "academico.php?modulo=principal/solicitacaoViagemExterior&acao=A&PopUpMinuta=1&aveid=" + aveid;
	//janela(url,800,600,"Minuta");
}
</script>
<div class='col-md-2' style='position:static;'>
    <?=cria_aba_2($abacod_tela, $url, $parametros);?>
</div>
<div class='col-md-9' style='position:static;'>
    <?
    echo $autoriazacaoconcursos->cabecalho_entidade($_SESSION['academico']['entid']);
    ?>
<form id="formulario" name="formulario" method="post" action="">
	<input type="hidden" value="salvarAutorizacao" name="requisicao" />
	<input type="hidden" value="<?php echo $aveid ?>" name="aveid" />
	<?php if($docid) {
	  $estado_documento = wf_pegarEstadoAtual( $docid );
	}?>
	<?php if($aveid): ?>
		<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td valign="top" class="SubtituloDireita" width="24%" >Solicitação:</td>
				<td valign="top" ><?php echo $aveid ?></td>
				<td valign="top" align="right" rowspan="4">
					<?php /* Barra de estado atual e ações e Historico */
//					$permissaoWorkflow = true;
//					if($estado_documento['esdid'] == WF_SOLICITACAO_VIAGEM_AGUARDANDO_AUTORIZACAO_REITOR && $_SESSION['usucpf'] != $avecpfreitor){
//						$permissaoWorkflow = false;	
//					}
//					if($permissaoWorkflow){
						wf_desenhaBarraNavegacao( $docid, array( 'aveid' => $aveid )); 
					//}?>
				</td>
			</tr>
			<tr>
				<td valign="top" class="SubtituloDireita" width="24%" >Reitor:</td>
				<td valign="top" ><?php echo $avenomereitor ?></td>
			</tr>
			<tr>
				<td valign="top" class="SubtituloDireita" width="24%" >Número de Autorização:</td>
				<td valign="top" ><?php echo $avenumauto ? $avenumauto : "N/A" ?></td>
			</tr>
			<?php if($estado_documento["esdid"] == WF_SOLICITACAO_VIAGEM_AUTORIZADO): ?>
				<tr>
					<td valign="top" class="SubtituloDireita" width="24%" >Visualizar Autorização</td>
					<td valign="top" ><a href="javascript:exibeMinuta('<?php echo $aveid ?>')"><img src="../imagens/print.gif" alt="Visualizar Impressão" class="link img_middle"  title="Visualizar Impressão" /> <?php echo $avenumauto ? $avenumauto : "N/A" ?></a></td>
				</tr>
			<?php endif; ?>
		</table>
	<?php endif; ?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php if($_REQUEST['requisicao'] && $_REQUEST['requisicao'] != "excluirAutorizacao"): ?>
			<tr class="center SubtituloTabela" >
				<td class="bold" width="70%" >Programa</td>
				<td class="bold" >Número de Participantes</td>
			</tr>
			<?php if($estado_documento["esdid"] == WF_SOLICITACAO_VIAGEM_EM_CADASTRAMENTO || !$estado_documento["esdid"]): ?>
				<?php $permissao = "S" ?>
			<?php else: ?>
				<?php $permissao = "N" ?>
			<?php endif; ?>
			<?php $arrProgramas = recuperaProgramaViagemExterior() ?>
			<?php $arrQtdeParc = $aveid ? recuperaProgramaViagemExteriorPorAveid($aveid) : array() ?>
			<?php if($arrProgramas): ?>
				<?php $i = 0; ?>
				<?php foreach($arrProgramas as $prog): ?>
					<?php $bgcolor = $i%2 ? "#FFFFFF" : "" ?>
					<tr bgcolor="<?php echo $bgcolor ?>" onmouseout="this.bgColor='<?php echo $bgcolor ?>';" onmouseover="this.bgColor='#ffffcc';" >
						<td><?php echo $prog['pvedsc'] ?></td>
						<td class="direita">
							<?php echo campo_texto("qtde_part[".$prog['pveid']."]","N",$permissao,'',12,12,"[.###]","","right","","","","",$arrQtdeParc[$prog['pveid']]); ?>
						</td>
					</tr>
					<?php $i++; ?>
				<?php endforeach; ?>
				<tr>
					<td class="center" colspan="2" >
						<?php if($estado_documento["esdid"] == WF_SOLICITACAO_VIAGEM_EM_CADASTRAMENTO || !$estado_documento["esdid"]): ?>
							<input type="button" value="Salvar" name="btn_salvar" />
							<input type="button" value="Cancelar" name="btn_cancelar" />
						<?php else: ?>
							<input type="button" value="Voltar" name="btn_voltar" onclick="window.location.href='academico.php?modulo=principal/solicitacaoViagemExterior&acao=A'" />
						<?php endif; ?>
					</td>
				</tr>
			<?php else: ?>
				<tr>
					<td class="center" colspan="2" >Não existe(m) programa(s) cadastrado(s).</td>
				</tr>
			<?php endif; ?>
	<?php else: ?>
		<tr class="center SubtituloTabela" >
			<td colspan="2" class="bold direita" width="30%" ><a href="javascript:cadastrarAutorizacao()" ><img src="../imagens/gif_inclui.gif" class="img_middle" /> Cadastrar Nova Solicitação</a></td>
		</tr>
		<?php if(!verificaEmailReitorPorEntid($_SESSION['academico']['entid'])): ?>
			<tr>
				<td colspan="2" class="center bold">
					<img src="../imagens/atencao.png" class="img_middle" /> Para solicitar viagens para o exterior é necessário que o Reitor esteja cadastrado. Por favor, cadastre o Reitor da Instituição no <?php echo SIGLA_SISTEMA; ?>.
				</td>
			</tr>
		<?php endif; ?>
		<tr>
			<td class="center SubtituloTabela bold" colspan="2" >Lista de Solicitações</td>
		</tr>
	<?php endif; ?>
	</table>
</form>
<?php if(!$_REQUEST['requisicao'] || $_REQUEST['requisicao'] == "excluirAutorizacao"): ?>
	<script type="text/javascript">
		function wf_exibirHistorico( docid )
		{
			var url = '../geral/workflow/historico.php' +
				'?modulo=principal/tramitacao' +
				'&acao=C' +
				'&docid=' + docid;
			window.open(
				url,
				'alterarEstado',
				'width=675,height=500,scrollbars=yes,scrolling=no,resizebled=no'
			);
		}
</script>
	<?php listaAutorizacaoViagem() ?>
<?php endif; ?>
</div>