<?php

header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

// Verifica se o 'memid' existe na sessão.
if(!$_SESSION['memid']) {
	echo "<script type=\"text/javascript\">
			alert(\"O cadastro de Diretor e Coordenador deve ser realizado para acessar esta aba.\");
			location.href = \"pdeescola.php?modulo=meprincipal/cadastro_diretor_coordenador&tipo=diretor&acao=A\";
		  </script>";
	exit;
}

include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
me_verificaSessao();

// Flag para determinar se a tela será de cadastro ou de consulta
$somenteConsulta = true;

$sql = "SELECT docid FROM pdeescola.memaiseducacao WHERE memid = ".$_SESSION["memid"];
$existeDocid = $db->pegaUm($sql);

if($existeDocid) {
	// Recupera ou cria o docid.
	$docid = meCriarDocumento( $_SESSION['meentid'] , $_SESSION['memid']  );
	if($docid == false) {
		echo "<script>
				alert('Esta entidade não consta na tabela principal do Mais Educação');
				history.back(-1);
			  </script>";
		die;
	}
}

$sql = "SELECT count(*) FROM seguranca.perfilusuario WHERE usucpf = '".$_SESSION["usucpf"]."' AND pflcod in (".PDEESC_PERFIL_CAD_MAIS_EDUCACAO.", ".PDEESC_PERFIL_SUPER_USUARIO.")";
$vPerfilCadMaisEscola = $db->pegaUm($sql);

if((integer)$vPerfilCadMaisEscola > 0) {
	if(!$existeDocid) {
		if((integer)$_SESSION["exercicio"] == meMaxProgramacaoExercicio()) {
			// Recupera ou cria o docid.
			$docid = meCriarDocumento( $_SESSION['meentid'], $_SESSION['memid']   );
			if($docid == false) {
				echo "<script>
						alert('Esta entidade não consta na tabela principal do Mais Educação');
						history.back(-1);
					  </script>";
				die;
			}
		} else {
			$docid = false;
		}
	}

	$esdid = mePegarEstadoAtual( $docid );
	
	if((integer)$esdid == CADASTRAMENTO_ME || (integer)$esdid == CORRECAO_CADASTRAMENTO_ME) {
		$somenteConsulta = false;
	}
}

if($_REQUEST['download'] == 'S'){
	$file = new FilesSimec("mearquivos", $campos  );
	$arqid = $_REQUEST['arqid'];
    $arquivo = $file->getDownloadArquivo($arqid);
    echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/quadras&acao=A';</script>";
    exit;
}   
echo "<br />";
echo montarAbasArray(carregaAbasMaisEducacao(), "/pdeescola/pdeescola.php?modulo=meprincipal/quadras&acao=A");

$titulo = "Mais Educação - Educação Integral";
$subtitulo = "Cadastro - Atividades ".((integer)$_SESSION["exercicio"] - 1);

monta_titulo($titulo, $subtitulo);

echo cabecalhoMaisEducacao();
 
$campos	= array("memid"		=> $_SESSION['memid'],
				"arpdata"	=>"'".date('Y-m-d')."'",
				"arpstatus" => "'A'"
				);	
$file = new FilesSimec("mearquivos", $campos ,"pdeescola");
if( $_POST['requisicao']){
	$sql = "DELETE FROM pdeescola.mequadra WHERE memid = ".$_SESSION['memid'];
	$del = $db->executar( $sql );
	if( $_POST['tsqid'] ){ 
		foreach( $_POST['tsqid'] as $tsqid ){
			$sql = "INSERT INTO pdeescola.mequadra (memid, tsqid) VALUES ( ".$_SESSION['memid'].", $tsqid )"; 
			$db->executar( $sql );
		}
		$db->commit();
	} 
	if($_FILES["Arquivo"]["name"]){	 
		$arquivoSalvo = $file->setUpload( "".$_POST['arqdescricao'].""); 
	}
	echo '<script type="text/javascript"> alert(" Operação realizada com sucesso!");</script>';
	echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/quadras&acao=A';</script>";
}
function checkItem($memid, $tsqid){
	 global $db;
	 $sql = "SELECT tsqid FROM pdeescola.mequadra WHERE memid = $memid AND tsqid = $tsqid ";
	 $tem = $db->pegaUm( $sql );
	 if( $tem ){
	 	echo "checked=checked";
	 }else{
	 	echo "";
	 }
}
if($_REQUEST['arqidDel'] != ''){
   
    $anexos = array();
    $sql = "UPDATE pdeescola.mearquivos SET arpstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);
    $sql = "UPDATE public.arquivo SET arqstatus = 'I' where arqid=".$_REQUEST['arqidDel'];
    $db->executar($sql);

    $db->commit();
    echo"<script>window.location.href = 'pdeescola.php?modulo=meprincipal/quadras&acao=A';</script>";
}
 
?> 
 
 
<script src="/obras/js/documentos.js" ></script>
 <form method="post" name="anexo" enctype="multipart/form-data" onsubmit="return ValidarFormulario(this);" action="">
	<br><center><B>Utilização dos Recursos para Reforma, Amplicação e Cobertura das Quadras Esportivas ou do Espaço Destinado ao Lazer.	</B></center><br> 
 	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center"> 
		<tr>
			<?
			$sql = "SELECT * FROM pdeescola.metipoobraquadra WHERE toqstatus = 'A'";
			$rsTipoObra = $db->carregar( $sql );
			if( $rsTipoObra ){
				foreach( $rsTipoObra as $tpObra ){?>
					<td valign="top"> 
						<b><?= $tpObra['toqdescricao']; ?>:</b><br><br><?
						$sqlS = "SELECT tsqdescricao, tsqid FROM pdeescola.metiposervicoquadra WHERE toqid = ".$tpObra['toqid']." AND tsqstatus = 'A'";
						$rsTipoServico = $db->carregar( $sqlS );					
						if( $rsTipoServico ){
							foreach( $rsTipoServico as $tpServico ){?>
								<input type="checkbox" name="tsqid[]" id="tsqid[]" value="<?=$tpServico['tsqid'];?>" <?=checkItem($_SESSION['memid'], $tpServico['tsqid'])?> >&nbsp; <?=$tpServico['tsqdescricao'];?><br>
							<?}
						}?> 
					</td>
				<?}
			}?>
		</tr>
	</table>
	
	<input type="hidden" name="requisicao" value="inserirarquivo"/>		
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center"> 
		<tr> 
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Imagem:</td>
			<td> 
				<input type="file" name="Arquivo"/>
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"/> 
			</td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita" style="vertical-align:top; width:25%">Descrição:</td>
			<td><?= campo_textarea( 'arqdescricao', 'S', $somenteLeitura, '', 60, 2, 250 ); ?></td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td>
				<input type="hidden" name="tpaid" id="tpaid" value="21"/>
				 
					<input type="submit" name="botao" value="Salvar"/>
				 
			</td>
		</tr>
	</table>
	<br/>
	

	
	<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
		<tr><td><div>
		<?
		$sql = "SELECT 
					arqnome, arq.arqid, 
					arq.arqextensao, arq.arqtipo, 
					arq.arqdescricao,
				 	to_char(arq.arqdata, 'DD/MM/YYYY') as data,
				 	'<img style=\"cursor:pointer; position:relative; z-index:10; top:-87px; left:-9px; float:right;\" src=\"../obras/plugins/imgs/delete.png\" border=0 title=\"Excluir\" onclick=\"javascript:ExcluirDocumento(\' acao=A&requisicao=excluir\',' || arq.arqid || ',' || meq.meaid || ');\">' as acao
				FROM 
					public.arquivo arq
				INNER JOIN 
					pdeescola.mearquivos meq ON meq.arqid = arq.arqid 
				WHERE 
					meq.memid= {$_SESSION["memid"]} AND
					arpstatus = 'A' AND
					(arqtipo = 'image/jpeg' OR arqtipo = 'image/gif' OR arqtipo = 'image/png') 
				ORDER BY 
					arq.arqid
				LIMIT 16 OFFSET ".($_REQUEST['pagina']*16);
		$fotos = ($db->carregar($sql));
		$_SESSION['downloadfiles']['pasta'] = array("origem" => "pdeescola","destino" => "pdeescola");
		
		if( $fotos ){
			$_SESSION['imgparams'] = array("filtro" => "cnt.memid=".$_SESSION["memid"]." AND 
														arpstatus = 'A'", 
										   "tabela" => "pdeescola.mearquivos ");
			
			for( $k=0; $k < count($fotos); $k++ ){
				echo "<div style=\"float:left; width:90px; height:100px; text-align:center; margin:3px;\" >
						<img border='1px' id='".$fotos[$k]["arqid"]."' src='../slideshow/slideshow/verimagem.php?newwidth=64&newheight=48&arqid=".$fotos[$k]["arqid"]."' hspace='10' vspace='3' style='position:relative; z-index:5; float:left; width:70px; height:70px;' onmouseover=\"return escape( '". $fotos[$k]["arqdescricao"] ."' );\" onclick='javascript:window.open(\"../slideshow/slideshow/index.php?pagina=". $_REQUEST['pagina'] ."&arqid=\"+this.id+\"\",\"imagem\",\"width=850,height=600,resizable=yes\")'/><br>
						" . $fotos[$k]["data"] . " <br/>
						" . $fotos[$k]["acao"] . "
					  </div>";
				
			}
			
		}else {
			echo "Não existem fotos cadastradas";
		}
		?>
		
		</div>
		</td></tr>
		<tr>
			<td align="center">
				<?
					if(!$_REQUEST['pagina']) $_REQUEST['pagina'] = 0;
					$sql = "SELECT COUNT(arq.arqid) AS totalregistros FROM pdeescola.mearquivos AS meq
							LEFT JOIN public.arquivo AS arq ON arq.arqid = meq.arqid 
							WHERE meq.memid = {$_SESSION["memid"]} AND 
							arpstatus = 'A' AND 
							(arqtipo = 'image/jpeg' OR
							 arqtipo = 'image/gif' OR
							 arqtipo = 'image/png')";
					$paginacao = current($db->carregar($sql));
					if($paginacao) {
						for($i = 0; $i < ceil(current($paginacao)/16); $i++ ) {
							$page[] = "<a href=?modulo=principal/album&acao=A&pagina=". $i .">".(($i==$_REQUEST['pagina'])?"<b>".($i+1)."</b>":($i+1))."</a>";
						}
						if(count($page) > 1) {
							echo implode(" | ", $page);
						}
					}
				?>
			</td>
		</tr>
		<tr>
			<td>
				
			</td>
		</tr>
	</table>	
</form>
<script>

function ExcluirDocumento(url, arqid, id){
	if( !arqid ){
		return false;
	}	
	if( !confirm( 'Deseja realmente excluir a imagem?' ) ){
		return false;
	}
	return window.location.href="pdeescola.php?modulo=meprincipal/quadras&acao=A&arqidDel="+arqid;
}
</script>