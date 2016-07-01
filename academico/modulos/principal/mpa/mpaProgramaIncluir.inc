<?php 
include_once APPRAIZ . "includes/classes/questionario/Tela.class.inc";
include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";

monta_titulo('Programa Incluir', 'Questionário');

$entid = $_SESSION['academico']['entid'];
$orgid = $_SESSION['academico']['orgid'];

switch($orgid){
	case '1':
		$funid = ACA_ID_CAMPUS;
	break;
	case '2':
		$funid = ACA_ID_UNED;
	break;
} 

function pegaQrpidProgIncluir( $entidcampus, $entid, $incano, $queid ){
	global $db;
    
    include_once APPRAIZ . "includes/classes/questionario/GerenciaQuestionario.class.inc";
   	
   	if($queid){
   		$aryWhere[] = "qr.queid = {$queid}";
   	}
   	
   	if($entid){
   		$aryWhere[] = "inc.entid = {$entid}";
   	}

   	if($entidcampus){
   		$aryWhere[] = "inc.entidcampus = {$entidcampus}";  
   	}
   	
   	if($incano){
	   	$aryWhere[] = "inc.incano = {$incano}";  	
   	}
   	
    $sql = "SELECT			inc.qrpid
            FROM		   	academico.incluirquestionario inc
            INNER JOIN     	questionario.questionarioresposta qr ON qr.qrpid = inc.qrpid
            				".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."";
            				
    $qrpid = $db->pegaUm($sql);
    
    if(!$qrpid){
    	$sql = "SELECT 	DISTINCT 'true'
    			FROM	questionario.questionario 		  
            	WHERE	queid = {$queid}";
            	
    	$testaQueid = $db->pegaUm($sql);
    	
    	if($testaQueid){
	        $sql = "SELECT		entnome
	                FROM	    entidade.entidade
	                WHERE	    entid = {$entid}";
	        
	        $titulo = $db->pegaUm($sql);
	        
	        $arParam = array ("queid" => $queid, "titulo" => "Acadêmico (".$titulo.")");
	        $qrpid = GerenciaQuestionario::insereQuestionario( $arParam );
	        
      		$sql = "INSERT INTO academico.incluirquestionario (entidcampus, entid, incano, qrpid) VALUES ({$entidcampus}, {$entid}, {$incano}, {$qrpid})";
	        $db->executar($sql);
	        $db->commit();
    	} else {
    		echo "<script>alert('Ação impossivel. Questionario inexistente.')</script>";
    		return false;
    	}
    }
    return $qrpid;
}

?>
<link rel="stylesheet" type="text/css" href="/includes/Estilo.css"/>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
jQuery('.adicionarQuestionario').live('click',function(){
	if($('#incano').val() == ''){
		alert('O campo "Ano" é obrigatório!');
		$('#incano').focus();
		return false;
	}
	if($('[name=entidcampus]').val() == ''){
		alert('O campo "Campus" é obrigatório!');
		$('[name=entidcampus]').focus();
		return false;
	}

    $('#req').val('adicionar');
    $('#formulario').submit();
});

function alterarQuestionario(entid,entidcampus,incano){
	$('#incano').val(incano);
	$('[name=entidcampus]').val(entidcampus);
	$('#req').val('alterar');
    $('#formulario').submit();
}
</script>

<form id="formulario" method="post" name="formulario" action="" onsubmit="" enctype="multipart/form-data">
	<input type="hidden" id="req" name="req" value=""/>
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="subtituloDireita">Ano:</td>
			<td><?php echo campo_texto('incano', 'N', 'S', 'Ano', '20', '4', '', '', '', '', '','id="incano"'); ?></td>
		</tr>	
		<tr>
			<td class="subtituloDireita">Campus:</td>
			<td>
	        	<?php $sql = "SELECT 		e.entid AS codigo,
											e.entnome AS descricao
							  FROM 			entidade.entidade e2 
							  INNER JOIN 	entidade.entidade e ON e2.entid = e.entid 
							  INNER JOIN 	entidade.funcaoentidade ef ON ef.entid = e.entid 
							  INNER JOIN 	entidade.funentassoc ea ON ea.fueid = ef.fueid 
							  LEFT JOIN 	entidade.endereco ed ON ed.entid = e.entid 
						 	  LEFT JOIN 	territorios.municipio mun ON mun.muncod = ed.muncod 
							  WHERE 		ea.entid = {$entid} AND e.entstatus = 'A' AND ef.funid = {$funid}
							  ORDER BY 		e.entnome"; 
	        	
	        	$db->monta_combo('entidcampus',$sql,'S','Selecione...','','','','372','S','','',$entidcampus); ?>			
			</td>
		</tr>	
	    <tr>
	        <td class="SubTituloEsquerda" colspan="2">
	      		<span class="adicionarQuestionario" style="cursor:pointer"><img border="0" style="vertical-align:middle;" src="../imagens/gif_inclui.gif">&nbsp;Adicionar Questionário</span>
	    	</td>
	    </tr>			
	</table>
</form>

<br>	

<?php 
if($_SESSION['baselogin'] == 'simec_desenvolvimento'){	
	$_SESSION['programa_incluir'] = 87;
} else {
	$_SESSION['programa_incluir'] = 88;
} ?>

<div id="div_questionario">
<?php 
$sql_perg = "SELECT     COUNT(p.perid)
			FROM 		questionario.pergunta p 
			INNER JOIN 	questionario.grupopergunta gp ON p.grpid = gp.grpid
			WHERE 		gp.queid = {$_SESSION['programa_incluir']}";

$qtdperg = $db->pegaUm($sql_perg);

$cabecalho = array('Ação', 'Campus', 'Ano','Situação');

$sql = "SELECT 			'<img src=\"../imagens/alterar.gif\" id=\"' || q.entidcampus ||'\" class=\"alterar\" onclick=\"alterarQuestionario('|| q.entid ||','|| q.entidcampus ||','|| q.incano ||');\" style=\"cursor:pointer;\"/>' AS acao,
						e.entnome AS campus,
						q.incano,
						CASE WHEN (COUNT(resdsc)::INTEGER = {$qtdperg}) THEN 'Completo' ELSE 'Incompleto' END AS situacao
		FROM 			academico.incluirquestionario q
		INNER JOIN		entidade.entidade e ON q.entidcampus = e.entid
		LEFT JOIN		questionario.resposta r ON r.qrpid = q.qrpid
		WHERE 			q.entid = {$entid}
		GROUP BY		q.entidcampus, q.entid, e.entnome, q.incano
		ORDER BY 		q.incano, e.entnome";

$db->monta_lista($sql, $cabecalho, '50','10', '', '', '', ''); ?>
</div>

<br>

<?php 
header('Content-type: text/html; charset=ISO-8859-1');
if($_REQUEST['req'] == 'adicionar' || $_REQUEST['req'] == 'alterar'){
	extract($_REQUEST);
	if($entidcampus){
		$sql = "SELECT	entnome FROM entidade.entidade WHERE entid = {$entidcampus}";
		$campus = $db->pegaUm($sql);
		$ano = "Questionário ".$incano;
		monta_titulo($campus, $ano); 
		$qrpid = pegaQrpidProgIncluir($entidcampus, $entid, $incano, $_SESSION['programa_incluir']);
		$tela = new Tela( array("qrpid" => $qrpid, 'tamDivArvore' => 25, 'habilitado' => 'S','relatorio'=> 'modulo=relatorio/mpaRelatorioProgramaIncluir&acao=A') ); 
	}
} 
?>	


<?php if($_REQUEST['req'] == 'adicionar'){ ?>
<script language="javascript" type="text/javascript">
jQuery('#div_questionario').load('academico.php?modulo=principal/monitoramentoProgramasAcoes&acao=A&aba=ProgramaIncluir #div_questionario');
</script>
<?php } ?>