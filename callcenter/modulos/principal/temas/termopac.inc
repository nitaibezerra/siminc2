<?php
//include '_funcoes_termo.php';

ini_set("memory_limit", "1024M");
set_time_limit(0);

if($_REQUEST['requisicao'] == 'salvar') {
	extract( $_POST );
	
	switch ($_SESSION['callcenter']['temid']) {
		case 1:
			if( !empty($proid) && !empty($ligid) ){
				$sql = "SELECT count(ateid) FROM par.atendimento WHERE proid = '$proid' and ligid = '$ligid'";
				$total = $db->pegaUm( $sql );
				
				if( $total == 0 ){
					$sql = "INSERT INTO par.atendimento( proid, ligid) 
							VALUES ('$proid', $ligid)";
					
					$db->executar( $sql );
					if($db->commit()){
						echo "<script> 
								window.location.href = window.location;
					  	  </script>";
					}
					exit;
				} else {
					$db->commit();
				}
			}
		break;
		
		default:
			;
		break;
	}
}
if( !empty($_GET['temid']) ) $_SESSION['callcenter']['temid'] = $_GET['temid'];

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

?>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="./js/par.js"></script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
		<tr>
			<td style="padding:15px; background-color:#e9e9e9; color:#404040; vertical-align: top;" colspan="4">
				<form method="post" name="formulario" id="formulario">
					<input type="hidden" name="requisicao" id="requisicao" value="salvar"  />
					<input type="hidden" name="proid" id="proid" value=""  />
					<input type="hidden" name="ligid" id="ligid" value=""  />
					<table border="0" cellpadding="3" cellspacing="0">
						<tr>
							<td valign="bottom">
								Município
								<br/>
								<?php 
									$filtro = simec_htmlentities( $_POST['municipio'] );
									$municipio = $filtro;
									echo campo_texto( 'municipio', 'N', 'S', '', 50, 200, '', ''); 
								?>
							</td>
							<td valign="bottom">
								Estado
								<br/>
								<?php
									$estuf = $_POST['estuf'];
									$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
									$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', '', '' );
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom" colspan="2">
								Resolução
								<br/>
								<?php 
									$resid = $_POST['resid'];
									$sql = "select resid as codigo, resdescricao as descricao from par.resolucao where resstatus='A'";
									$db->monta_combo( "resid", $sql, 'S', 'Todas as Resolução', '', '' ); 
								?>
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Tipo de Obra:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['tipoobra'] == "P" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="P" >Proinfancia<input <?php echo $_REQUEST['tipoobra'] == "Q" ? "checked='checked'" : "" ?> type="radio" name="tipoobra" value="Q" >Quadra
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Termo:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['termogerado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="1" >Termo Gerado<input <?php echo $_REQUEST['termogerado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="termogerado" value="2" >Termo Não Gerado
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Assinatura:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['assinado'] == "1" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="1" >Assinado<input <?php echo $_REQUEST['assinado'] == "2" ? "checked='checked'" : "" ?> type="radio" name="assinado" value="2" >Não Assinado
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								Ligações:
							</td>
						</tr>
						<tr>
							<td valign="bottom">
								<input <?php echo $_REQUEST['ligacao'] == "2" ? "checked='checked'" : "" ?> type="radio" name="ligacao" value="2" >Recebida<input <?php echo $_REQUEST['ligacao'] == "1" ? "checked='checked'" : "" ?> type="radio" name="ligacao" value="1" >Efetuada
							</td>
						</tr>
					</table>
					<div style="float: right;">
						<input type="submit" name="pesquisar" value="Pesquisar" />
						<input type="button" name="todos" value="Ver todos" onclick="window.location='<?=$_SERVER['HTTP_REFERER']; ?>';" />
					</div>
				</form>
			</td>
		</tr>
	</tbody>
</table>
<?php
if($_REQUEST['pesquisar']) {
	$inner = '';
	if($_REQUEST['municipio']) {
		$where[] = "removeacento(m.mundescricao) ILIKE removeacento('%".$_REQUEST['municipio']."%')";
	}
	
	if($_REQUEST['estuf']) {
		$where[] = "m.estuf='".$_REQUEST['estuf']."'";
	}
	if($_REQUEST['termogerado'] == "1") {
		//$inner = "INNER JOIN par.termocompromissopac t ON t.muncod = m.mucndo";
		$where[] = "( SELECT 
							t.terstatus
						FROM 
							par.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM par.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) = 'A'";
	}
	if($_REQUEST['termogerado'] == "2") {
		$where[] = "(
						( SELECT 
							t.terstatus
						FROM 
							par.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM par.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) = 'I'

						OR
						
						( SELECT 
							t.terstatus
						FROM 
							par.termocompromissopac t
						where 
							t.muncod = m.muncod AND t.terid = (SELECT max(terid) FROM par.termocompromissopac t2 WHERE t2.muncod = t.muncod)
						limit 1 ) IS NULL
						
						)";
//		$where[] = "(m.muncod NOT IN ( SELECT DISTINCT t.muncod FROM par.termocompromissopac t ) OR m.muncod IN ( SELECT DISTINCT t.muncod FROM par.termocompromissopac t WHERE t.terstatus = 'I' ))";
	}
	if($_REQUEST['tipoobra'] == "P") {
		$where[] = "protipo = 'P'";
	}
	if($_REQUEST['tipoobra'] == "Q") {
		$where[] = "protipo <> 'P'";
	}
	if($_REQUEST['assinado'] == "1") {
		$where[] = "ter.terassinado = TRUE";
	}
	if($_REQUEST['assinado'] == "2") {
				$where[] = "( ter.terassinado = FALSE OR ter.terassinado IS NULL )";
	}
	if($_REQUEST['resid']) {
		$where[] = "r.resid = '".$_REQUEST['resid']."'";
	}
	if( $_REQUEST['ligacao'] == 2 ){
		$where[] = "lig.tlgid = '".$_REQUEST['ligacao']."'";
		$join = " inner join par.atendimento ate on ate.proid = p.proid 
				  inner join callcenter.ligacao lig on lig.ligid = ate.ligid ";
	}
	if( $_REQUEST['ligacao'] == 1 ){
		$where[] = "lig.tlgid = '".$_REQUEST['ligacao']."'";
		$join = " inner join par.atendimento ate on ate.proid = p.proid 
				  inner join callcenter.ligacao lig on lig.ligid = ate.ligid ";
	}
	
}

$cabecalho = array("Ações","Nº Processo","Resolução","Município","UF", "Tipo obra","Valor total(R$)","Valor empenhado(R$)","Assinado");

$sql = "SELECT 
			'<center>
			 <img src=../imagens/telefone_vermelho.png style=cursor:pointer; title=\"Recebida\" height=\"22px\" onclick=\"iniciaAtendimento( '||p.proid||', 2 );\");\">
			 <img src=../imagens/telefone_verde.png style=cursor:pointer; title=\"Efetuada\" height=\"22px\" onclick=\"iniciaAtendimento( '||p.proid||', 1 );\");\">
			 </center>' as acoes, 
		   	p.pronumeroprocesso, 
		   	COALESCE(r.resdescricao,'N/A') as resdescricao,
		   	m.mundescricao, 
		   	m.estuf, 
		   	CASE WHEN protipo='P' THEN 'Proinfância' ELSE 'Quadra' END as tipoobra,
				CASE WHEN protipo='P' 
					THEN 
						(
						SELECT sum(round(po.prevalorobra,2))  
						FROM obras.preobra po 
						INNER JOIN par.empenhoobra eo ON eo.preid = po.preid and eobstatus = 'A' 
						INNER JOIN par.empenho emp ON emp.empid = eo.empid  and empstatus = 'A'
						INNER JOIN workflow.documento d ON d.docid=po.docid 
						WHERE emp.empnumeroprocesso=p.pronumeroprocesso
						AND po.prestatus='A' 
						AND d.esdid=228
						AND ptoid <> 5
						)
					ELSE
						(
						SELECT sum(round(po.prevalorobra,2))  
						FROM obras.preobra po 
						INNER JOIN par.empenhoobra eo ON eo.preid = po.preid and eobstatus = 'A'
						INNER JOIN par.empenho emp ON emp.empid = eo.empid and empstatus = 'A' 
						INNER JOIN workflow.documento d ON d.docid=po.docid 
						WHERE emp.empnumeroprocesso=p.pronumeroprocesso
						AND po.prestatus='A' 
						AND d.esdid=228
						AND ptoid = 5
						)
					END as valortotal,			   
		   	(SELECT sum(ep.empvalorempenho) FROM par.empenho ep WHERE ep.empnumeroprocesso=p.pronumeroprocesso and empstatus = 'A') as valorempenhado,
			 CASE WHEN ter.terassinado=TRUE THEN '<center>Assinado</center>' ELSE '<center>Termo não assinado</center>' END as assinado
		FROM 
			par.processoobra p 
		LEFT JOIN par.resolucao r ON r.resid=p.resid
		INNER JOIN territorios.municipio m ON m.muncod=p.muncod 
		LEFT JOIN par.termocompromissopac ter ON ter.proid = p.proid AND ter.terstatus='A' 
		$join
		WHERE 1=1
		and p.prostatus = 'A'
	".(($where)?"AND ".implode(" AND ", $where):"")."";

$db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
?>
<script>
	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url) {
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(690,400);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	function displayStaticMessage(messageContent,cssClass) {
		messageObj.setHtmlContent(messageContent);
		messageObj.setSize(600,150);
		messageObj.setCssClassMessageBox(cssClass);
		messageObj.setSource(false);	// no html source since we want to use a static message here.
		messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
		messageObj.display();
	}
	function closeMessage() {
		messageObj.close();	
	}
	
	function iniciaAtendimento(proid,tlgid){
		//function abrir(pagina,largura,altura) {
		
		var altura = 640;
		var largura = 800;
		var pagina = 'callcenter.php?modulo=principal/popupAtendimento&acao=A&proid='+proid+'&tlgid='+tlgid;
		//pega a resolução do visitante
		w = screen.width;
		h = screen.height;
		
		//divide a resolução por 2, obtendo o centro do monitor
		meio_w = w/2;
		meio_h = h/2;
		
		//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
		altura2 = altura/2;
		largura2 = largura/2;
		/*meio1 = meio_h-altura2;
		meio2 = meio_w-largura2;*/
		meio1 = 10;
		meio2 = 5;
		
		//abre a nova janela, já com a sua devida posição
		window.open(pagina,'','height=' + altura + ', width=' + largura + ',scrollbars=yes, top='+meio1+', left='+meio2+''); 
		//}
		/*window.open('callcenter.php?modulo=principal/popupAtendimento&acao=A&proid='+proid, 
				    'Abertura de Atendimento', 
				    "height=840,width=970,scrollbars=yes,top=50,left=200" ).focus();*/
	}
	function salvarAtendimento( ligid, proid ){
		window.document.getElementById('proid').value = proid;
		window.document.getElementById('ligid').value = ligid;
		window.document.getElementById('requisicao').value = 'salvar';
		
		window.document.getElementById('formulario').submit();
	}
</script>
