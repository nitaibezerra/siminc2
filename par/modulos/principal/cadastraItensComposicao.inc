<?php

if($_POST['req'] == 'verificaPregaoEstadoData'){
	header('content-type: text/html; charset=ISO-8859-1');

	extract($_POST);

	$dtinip = formata_data_sql($dicdatainicial);
    $dtfimp = formata_data_sql($dicdatafinal);
    $andPeriodo = "	         AND (
                                        (
                                        to_char(dic.dicdatainicial, 'YYYY-MM-DD') BETWEEN '$dtinip'and '$dtfimp'
                                         or
                                        to_char(dic.dicdatafinal, 'YYYY-MM-DD')  BETWEEN '$dtinip' and '$dtfimp'
                                        )
                                        or
                                        (
                                        '$dtinip' BETWEEN to_char(dic.dicdatainicial, 'YYYY-MM-DD') and to_char(dic.dicdatafinal, 'YYYY-MM-DD')
                                         or
                                        '$dtfimp' BETWEEN to_char(dic.dicdatainicial, 'YYYY-MM-DD') and to_char(dic.dicdatafinal, 'YYYY-MM-DD')
                                        )
                                      )
                               ";

	$sql = "select distinct
				dic.picid
			from
				par.detalheitemcomposicao dic
			inner join
				par.propostaitemcomposicao pic ON pic.picid = dic.picid
			inner join
				par.ufdetalheitemcomposicao ufd ON ufd.dicid = dic.dicid
			where
				pic.picid = $picid
				and dic.dicstatus = 'A'
				and	pic.picstatus = 'A'
				".($estuf ? " and estuf in ('".implode("','",$estuf)."') " : "" )."
				".($dicid ? " and dic.dicid != $dicid" : "" )."
				$andPeriodo
			";

	$dicid = $db->pegaUm($sql);
	if(!empty($dicid)){
		echo 'N';
	}else{
		echo 'S';
	}
	die();
}

if( $_POST['req'] == 'listaItens' ){
	header('content-type: text/html; charset=ISO-8859-1');
	listaItens();
	die();
}

if( $_POST['req'] == 'listaPeriodos' ){
	header('content-type: text/html; charset=ISO-8859-1');
	listaPeriodos( $_POST['picid'] );
	die();
}

if( $_POST['req'] == 'listaPeriodosPrincipal' ){
	header('content-type: text/html; charset=ISO-8859-1');
	echo "";
	listaPeriodos( $_POST['picid'] );
	echo "";
	listaTipoSubacao( $_POST['picid'] );
	echo "";
	die();
}

if( $_POST['req'] == 'listaUfs' ){
	header('content-type: text/html; charset=ISO-8859-1');
	listaUfs( $_POST['dicid'] );
	die();
}

if( $_POST['req'] == 'carregaItem' ){
	header('content-type: text/html; charset=ISO-8859-1');
	if( $_POST['picid'] ){

		$existeItemEmenda = $db->pegaUm("select case when count(ip.picid) > 0 then 'S' else 'N' end from emenda.itempar ip
											inner join emenda.itempar_especificacao ie on ie.ipaid = ip.ipaid
										where ip.picid = {$_POST['picid']}");
		$sql = 'SELECT
					picdescricao,
					picdetalhe,
					pictipodespesa,
					umiid,
					picemendas,
					picpregao
				FROM
					par.propostaitemcomposicao
				WHERE
					picid = '.$_POST['picid'];
		$arr = $db->pegaLinha($sql);
		$dados = $arr['picdescricao'] .'|||'. $arr['picdetalhe'] .'|||'. $arr['pictipodespesa'] .'|||'. $arr['umiid'].'|||'.$arr['picemendas'].'|||'.$existeItemEmenda.'|||'.$arr['picpregao'];

		$sql = "SELECT
					pts.ptsid,
					pts.ptsdescricao || ' - ' || fe.frmdsc as ptsdescricao
				FROM
					par.propostaitemtiposubacao pits
				INNER JOIN par.propostatiposubacao pts ON pts.ptsid = pits.ptsid
				INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
				WHERE
					picid = ".$_POST['picid'];
		$ptsids = $db->carregar($sql);
		if(is_array($ptsids)){
			foreach($ptsids as $ptsid){
				$dados .=  '|||' . $ptsid['ptsid'] . '|||' . $ptsid['ptsdescricao'];
			}
		}
		echo $dados;
		die;

	}
	die();
}

if( $_POST['req'] == 'carregaPeriodo' ){
	header('content-type: text/html; charset=ISO-8859-1');
	if( $_POST['dicid'] ){
		$sql = 'SELECT
					ptp.ptpid,
					ptp.ptpnumeroata,
					dic.dicpublicado,
					dic.dicvalor,
					dic.dicpercentual,
					dic.dicdatainicial,
					dic.dicdatafinal,
                    dic.dicpregao
				FROM
					par.detalheitemcomposicao dic
				LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid
				WHERE
					dicid = '.$_POST['dicid'];
		$arr = $db->pegaLinha($sql);
		$sql = 'SELECT
					est.estuf,
					est.estdescricao
				FROM
					par.ufdetalheitemcomposicao udic
				INNER JOIN territorios.estado est ON est.estuf = udic.estuf
				WHERE
					dicid = '.$_POST['dicid'];
		$ufs = $db->carregar($sql);
		if(is_array($ufs)){
			foreach($ufs as $k => $uf){
				$ufs[$k]['estdescricao'] = $uf['estdescricao'];
			}
		}
		$arr['ufs'] = $ufs;
		echo simec_json_encode($arr);
	}
	die();
}

if( $_POST['req'] == 'salvarItem' ){
	header('content-type: text/html; charset=ISO-8859-1');

	$_POST['picpregao'] = $_POST['picpregao'] == 'TRUE' ? $_POST['picpregao'] : 'FALSE';
	
	if( $_POST['picid'] == '' ){

		$sql = 'INSERT INTO par.propostaitemcomposicao(
            		picdescricao,
            		picdetalhe,
            		pictipodespesa,
					umiid,
					picemendas,
            		picstatus,
					picpregao)
				VALUES (
					\''.ICONV( "UTF-8", "ISO-8859-1", $_POST['picdescricao']) .'\',
					\''.ICONV( "UTF-8", "ISO-8859-1", $_POST['picdetalhe']).'\',
					\''.$_POST['pictipodespesa'].'\',
					'.$_POST['umiid'].',
					\''.$_POST['picemendas'].'\',
					\'A\',
					'.$_POST['picpregao'].')
				RETURNING
					picid;';

		$picid = $db->pegaUm($sql);

	}else{

		$sql = 'UPDATE par.propostaitemcomposicao
				SET
					picdescricao    = \''.ICONV( "UTF-8", "ISO-8859-1", $_POST['picdescricao']).'\',
            		picdetalhe 	    = \''.ICONV( "UTF-8", "ISO-8859-1", $_POST['picdetalhe']).'\',
            		pictipodespesa  = \''.$_POST['pictipodespesa'].'\',
            		umiid 		    = '.$_POST['umiid'].',
            		picemendas	    = \''.$_POST['picemendas'].'\',
            		picpregao		= '.$_POST['picpregao'].'
            	WHERE
            		picid = '.$_POST['picid'];

		$db->executar($sql);

		$picid = $_POST['picid'];

	}

	if(is_array($_POST['ptsid'])){

		$db->executar('DELETE FROM par.propostaitemtiposubacao WHERE picid = '.$picid);

		foreach($_POST['ptsid'] as $id){

			if( (float)$id > 0 ){
				$sql = 'INSERT INTO par.propostaitemtiposubacao(
							picid, ptsid)
					    VALUES (
					    	'.$picid.',\''.$id.'\');';
				$db->executar($sql);
			}
		}
	}
	$db->commit();

	$arr['picid'] = $picid;

	echo simec_json_encode($arr);
	die();
}

if( $_POST['req'] == 'excluirItem' ){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = 'UPDATE par.propostaitemcomposicao
				SET
					picstatus = \'I\'
            	WHERE
            		picid = '.$_POST['picid'];

	$db->executar($sql);

	$db->commit();

	die();
}

if( $_REQUEST['req'] == 'telaTipoPregao') {
	header('content-type: text/html; charset=ISO-8859-1');
	echo  '<html>
			 <head>
			 <meta http-equiv="Cache-Control" content="no-cache">
			 <meta http-equiv="Pragma" content="no-cache">
			 <meta http-equiv="Connection" content="Keep-Alive">
			 <meta http-equiv="Expires" content="-1">
			 <title>Selecione tipo pregão</title>
			 <script language="JavaScript" src="../../includes/funcoes.js"></script>
			 <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
			 </head>';

	echo '<form method=post action=par.php?modulo=principal/cadastraItensComposicao&acao=A&req=telaTipoPregao>
		  <script>
		  function selecionarNumeroAta(ptpid,ptpnumeroata) {
			  window.opener.document.getElementById(\'ptpid\').value=ptpid;
			  window.opener.document.getElementById(\'ptpnumeroata\').value=ptpnumeroata;
			  window.close();
		  }
		  </script>
			  <table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			  <tr>
			  	<td class="SubTituloDireita">Número da ata:</td>
			  	<td><input type=text size=50 name=ptpnumeroata id=ptpnumeroata></td>
			  </tr>
			  <tr>
			  	<td class="SubTituloCentro" colspan="2"><input type=submit name=buscarnumeroata value=Buscar></td>
			  </tr>
			  </table>
			  </form>';


	$ptpid = $_POST['ptpid'];
	$sql = "SELECT
				'<a style=cursor:pointer; onclick=selecionarNumeroAta(\''||ptpid||'\',\''||ptpnumeroata||'\');>'||ptpnumeroata||'</a>' as descricao
			FROM
				par.propostatipopregao
			WHERE
				ptpstatus = 'A'
			ORDER BY
				ptpanoata,
				ptpdatainicio,
				ptpvigencia	";

	$cabecalho = array("Descrição");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2);


	echo '</html>';
	die($html);

}


if( $_POST['req'] == 'salvarPeriodo' ){
	header('content-type: text/html; charset=ISO-8859-1');

	extract($_POST);

	if( !$dicid ){

		$sql = 'SELECT
					count(*)
				FROM
					par.detalheitemcomposicao
				WHERE
					picid = '.$picid;

		$dicperiodo = $db->pegaUm($sql);
		$dicperiodo++;

		$dicperiodo = $dicperiodo ? $dicperiodo : 1;

		$valpercent = $dicpercentual ? $dicpercentual : "NULL";

		if($pregao){
			$variavel = "";
			$valor = "";
			if( $pregaopublicado == 1 ){
				$variavel = "ptpid, ";
				$valor = $ptpid.",";
			} else {
				$varPerc = "dicpercentual,";
				$dadoPerc = $valpercent.' ,';
			}


			$sql = 'INSERT INTO par.detalheitemcomposicao(
			            dicperiodo,
			            picid,
			            '.$variavel.'
			            dicvalor,
			            dicpublicado,
			            '.$varPerc.'
			            dicdatainicial,
			            dicdatafinal,
                        dicpregao
                        )
				    VALUES (
				    	'.$dicperiodo.',
				    	'.$picid.',
			            '.$valor.'
				    	'.((float)str_replace(array('.',','),array('','.'),$dicvalor)).',
				    	'.$pregaopublicado.',
				    	'.$dadoPerc.'
				    	\''.formata_data_sql($dicdatainicial).'\',
				    	\''.formata_data_sql($dicdatafinal).'\',
                        '.$pregao.'
                        )
				    RETURNING
				    	dicid';
		}else{
			$sql = 'INSERT INTO par.detalheitemcomposicao(
			            dicperiodo,
			            picid,
			            ptpid,
			            dicvalor,
			            dicpercentual,
			            dicdatainicial,
			            dicdatafinal,
                        dicpregao
                        )
				    VALUES (
				    	'.$dicperiodo.',
				    	'.$picid.',
				    	NULL,
				    	'.str_replace(array('.',','),array('','.'),$dicvalor).',
				    	'.$valpercent.',
				    	\''.formata_data_sql($dicdatainicial).'\',
				    	\''.formata_data_sql($dicdatafinal).'\',
                        '.$pregao.'
                        )
				    RETURNING
				    	dicid';
		}
		$dicid = $db->pegaUm($sql);

	} else {

		$ptpid 	    	= $ptpid 	  	  ? $ptpid 												: 'NULL';
		$dicvalor 	    = $dicvalor 	  ? str_replace(array('.',','),array('','.'),$dicvalor) : 'NULL';
		$dicpercentual  = $dicpercentual  ? $dicpercentual 										: 'NULL';
		$dicdatainicial = $dicdatainicial ? formata_data_sql($dicdatainicial) 					: 'NULL';
		$dicdatafinal   = $dicdatafinal   ? formata_data_sql($dicdatafinal) 					: 'NULL';

		$sql = 'UPDATE par.detalheitemcomposicao
				SET
		            ptpid		   = '.$ptpid.',
					dicvalor       = '.$dicvalor.',
            		dicpercentual  = '.$dicpercentual.',
            		dicpublicado   = '.$pregaopublicado.',
            		dicdatainicial = \''.$dicdatainicial.'\',
            		dicdatafinal   = \''.$dicdatafinal.'\',
                    dicpregao = '.$pregao.'
            	WHERE
            		dicid = '.$dicid;

		$db->executar($sql);
	}

	if(is_array($estuf)){

		$db->executar('DELETE FROM par.ufdetalheitemcomposicao WHERE dicid = '.$dicid);

		foreach($estuf as $uf){

			$sql = 'INSERT INTO par.ufdetalheitemcomposicao(
						dicid, estuf)
				    VALUES (
				    	'.$dicid.',\''.$uf.'\');';
			$db->executar($sql);
		}
	}

	$db->commit();

	if(!empty($dicid)){
		echo "S";
	}else{
		echo "N";
	}
	die();
}

if( $_POST['req'] == 'excluirPeriodo'){
	header('content-type: text/html; charset=ISO-8859-1');
	$sql = "UPDATE 	par.detalheitemcomposicao
			SET     dicstatus = 'I'
            WHERE	dicid = {$_POST['dicid']}";

	$db->executar($sql);

	if($db->commit()){
		echo "S";
	} else {
		echo "N";
	}
	die();
}

function listaItens(){
	global $db;

	$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","Item","Detalhamento","Tipo Despesa","Unidade de Medida");

	$and = '';
	if($_REQUEST['picdescricao']){
        $picdescricaoTmp = removeAcentos(str_replace("-"," ",$_REQUEST['picdescricao']));
		$and = "AND UPPER(public.removeacento(picdescricao)) ILIKE '%".$picdescricaoTmp."%'";
	}
	if($_REQUEST['picid']){
		$and = "and picid = ".$_REQUEST['picid'];
	}


	$sql = "SELECT
				'<img src=\"../imagens/mais.gif\"title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarListaPeriodos(\''||picid||'\',this);\">' as mais,
				'<center><img src=../imagens/alterar.gif title=mais style=cursor:pointer; onclick=\"carregarItem(\''||picid||'\');\"></center>' as editar,
				'<center><img src=../imagens/excluir.gif title=mais style=cursor:pointer; onclick=\"excluirItem(\''||picid||'\');\"></center>' as excluir,
				picdescricao,
				picdetalhe,
				CASE WHEN pictipodespesa = '1'
					THEN 'Custeio'
					ELSE 'Capital'
				END as tipodespesa,
				und.umidescricao
			FROM
				par.propostaitemcomposicao pic
			INNER JOIN par.unidademedidadetalhamentoitem und ON und.umiid = pic.umiid
			WHERE
				picstatus = 'A'
				$and
			ORDER BY pic.picid";
	$arrDados = $db->carregar($sql);
	if($arrDados){
		$x = 0;
		foreach($arrDados as $dado){
			$arrDados[$x]['picdescricao'] = $arrDados[$x]['picdescricao'];
			$arrDados[$x]['picdetalhe'] = $arrDados[$x]['picdetalhe'];
			$x++;
		}
	}
	$arrDados = is_array($arrDados) ? $arrDados : Array();
	return $db->monta_lista($arrDados,$cabecalho,100,5,'N','center',"N","formprocesso");
}

function listaPeriodos( $picid ){

	global $db;

	if( $picid ){
		$cabecalho = array("&nbsp;","&nbsp;","&nbsp;","Detalhamento","Pregão", "SIGARP", "Valor","Percentual","Data Inicial","Data Final");

		$sql = "SELECT DISTINCT
					'<center><img src=../imagens/mais.gif title=mais style=cursor:pointer; onclick=\"carregarUFPeriodo(\''||dic.dicid||'\',this);\"></center>' as mais,
					'<center><img src=../imagens/alterar.gif title=mais style=cursor:pointer; onclick=\"carregarPeriodo(\''||dic.dicid||'\');\"></center>' as editar,
					CASE WHEN sic.dicid IS NULL 
                                            THEN '<center><img src=../imagens/excluir.gif title=mais style=cursor:pointer; onclick=\"excluirPeriodo(\''||dic.dicid||'\');\"></center>' 
                                            ELSE '<center><img src=../imagens/excluir_01.gif style=cursor:pointer; title=\"Detalhe do Item em uso nas subações.\"></center>'  
                                        END as excluir,
					dic.dicperiodo,
					CASE WHEN ptp.ptpnumeroata is null
						THEN '---'
						ELSE ptp.ptpnumeroata||''
					END as pregao,
                    pic.idsigarp,
					CASE WHEN dic.dicvalor is null
						THEN '---'
						ELSE dic.dicvalor||''
					END as valor,
					CASE WHEN dic.dicpercentual is null
						THEN '---'
						ELSE dic.dicpercentual || '%'
					END as perc,
					to_char(dic.dicdatainicial,'DD/MM/YYYY') as dataini,
					to_char(dic.dicdatafinal,'DD/MM/YYYY') as datafim
				FROM
					par.detalheitemcomposicao dic
				LEFT JOIN par.propostatipopregao ptp ON ptp.ptpid = dic.ptpid
                LEFT JOIN par.propostaitemcomposicao pic ON dic.picid = pic.picid
                LEFT JOIN par.subacaoitenscomposicao sic ON sic.picid = pic.picid AND sic.icostatus = 'A' AND sic.dicid = dic.dicid
				WHERE
					dic.picid = ".$picid."
					AND dic.dicstatus = 'A'";

		return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
	}
}

function listaTipoSubacao( $picid ){

	global $db;

	if( $picid ){
		$cabecalho = array("Tipo de Subação");

		$sql = "SELECT
					pts.ptsdescricao
				FROM
					par.propostaitemtiposubacao pits
				INNER JOIN par.propostatiposubacao pts ON pts.ptsid = pits.ptsid
				WHERE
					picid = ".$picid;

		return $db->monta_lista($sql,$cabecalho,150,5,'N','center','N','formprocesso');
	}
}

function listaUfs( $dicid ){

	global $db;

	if( $dicid ){
		$cabecalho = array("Unidades Federativas");

		$sql = "SELECT
					est.estdescricao||' - '||est.estuf as estado
				FROM
					par.ufdetalheitemcomposicao ufdic
				INNER JOIN territorios.estado est ON est.estuf = ufdic.estuf
				WHERE
					dicid = ".$dicid;

		return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
	}
}


if( !$_GET['carregaitem'] ){
	require_once APPRAIZ . 'includes/cabecalho.inc';
} else { ?>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<script type="text/javascript" src="../includes/funcoes.js" ></script>
<?php }
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function pesqItem(){
	if(document.formItem.picdescricao.value == ''){
		alert("Campo de Pesquisa Item obrigatório");
		return false;
		}
	document.formItem.submit();
}

function formataData(data){
	var arrData    = data.split('-');
	var Data = arrData[2]+'/'+arrData[1]+'/'+arrData[0];

	return Data;
}

function formataValor(valor){

	return valor.replace('.',',');
}

function carregarListaPeriodos(picid, obj) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=listaPeriodosPrincipal&picid="+picid,
			async: false,
			success: function(msg){
			ncol.innerHTML="<div id='listatermoprocesso_" + picid + "' >" + msg + "</div>";
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function carregarUFPeriodo(dicid, obj) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=listaUfs&dicid="+dicid,
			async: false,
			success: function(msg){
			ncol.innerHTML="<div id='listatermoprocesso_" + dicid + "' >" + msg + "</div>";
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function carregarLista(picid) {
	if(!picid) picid = "";
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=listaItens&picid="+picid,
	   	dataType: 'html',
	   	success: function(msg){
	    	$('#lista').html(msg);
	   	}
	 });
}

function carregarListaPeriodo() {
	var picid = $('#picid').val();
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=listaPeriodos&picid="+picid,
	   	dataType: 'html',
	   	success: function(msg){
//	   		alert(msg);
	    	$('#listaPeriodo').html(msg);
	   	}
	 });
}

function carregarPeriodo( dicid ) {
	travarBotoesItem();
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=carregaPeriodo&dicid="+dicid,
	   	dataType: 'json',
	   	success: function(msg){

	    	$('#dicid').val(dicid);

            // Carrega a opção pregao que foi salva na base de dados
            $('input[name="pregao"]').each(function(){
                if($(this).val()==msg.dicpregao){
                    $(this).attr('checked',true);
                }
            });

	    	if(msg.ptpid==null){
				$('#dicpercentual').val(msg.dicpercentual);
				if( msg.dicpublicado == null ){
                    $('#trPregaoPublicado').hide();
				} else {
					if( msg.dicpublicado == 0 ){
						$('#trPregaoPublicado').show();
						$('input[name="pregaopublicado"]').each(function(){
							if($(this).val()==msg.dicpublicado){
								$(this).attr('checked',true);
							}
						});
						$('#trPregao').hide();
					} else {
						$('#trPregao').show();
					}
				}
				$('#trValor').show();
				$('#trPercentual').show();
		    }else{
				$('#trPregao').show();
				$('#trPregaoPublicado').show();
				$('input[name="pregaopublicado"]').each(function(){
					if($(this).val()=="1"){
						$(this).attr('checked',true);
					}
				});
				$('#trPercentual').hide();
				$('#ptpid').val(msg.ptpid);
				$('#ptpnumeroata').val(msg.ptpnumeroata);
				//$('input[name="pregaopublicado"]').val(msg.dicpublicado);
			}
	    	$('#dicvalor').val(formataValor(msg.dicvalor));
			$('#dicdatainicial').val(formataData(msg.dicdatainicial));
	    	$('#dicdatafinal').val(formataData(msg.dicdatafinal));
	    	if($('#estuf_campo_off').css('display')!='none'){
		    	$('#estuf_campo_off').click();
		    }
	    	$('#estuf').children().remove();
	    	$(msg.ufs).each(function(){
		    	$('#estuf').append('<option value="'+this.estuf+'">'+this.estdescricao+'</option>');
		    });
	    	$('#novoPeriodo').show();
	    	carregarLista($('#picid').val());
	    	carregarListaPeriodo();
	   	}
	 });
}

function excluirPeriodo( dicid ) {
	if( confirm('Deseja exluir este periodo?') ){
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=excluirPeriodo&dicid="+dicid,
		   	dataType: 'html',
		   	success: function(msg){
			   	if($.trim(msg) == "N"){
			   		alert('Periodo não excluido.');
			   	} else {
			   		alert('Periodo excluido.');
			    	$('#novoPeriodo').show();
			    	carregarLista($('#picid').val());
			    	carregarListaPeriodo();
			   	}
			}
		 });
	}
}

function carregarItem( picid ){
	if(!picid){
		alert("Falta o id do item");
		return false;
	}
	$( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=carregaItem&picid="+picid,

	   	success: function(msg){
	   		var dados = msg.split('|||');

	    	$('#picid').val(picid);
	    	$('#picdescricao').val(dados[0]);
	    	if($('#ptsid_campo_off').css('display')!='none'){
		    	$('#ptsid_campo_off').click();
		    }
	    		$('#ptsid').children().remove();

	    	for(i=0;i<dados.length;i++){
		    	if(i > 6){
					$('#ptsid').append('<option value="'+dados[i]+'">'+dados[i+1]+'</option>');
					i++;
		    	}
			}

			$('#picdetalhe').val(dados[1]);
			$('#pictipodespesa').val(dados[2]);
			$('#umiid').val(dados[3]);
			if(dados[4] == 'S') $('#picemendassim').attr('checked',true);
			if(dados[4] == 'N') $('#picemendasnao').attr('checked',true);
			$('#existeitememenda').val(dados[5]);
			if(dados[6] == 't') $('#picpregaosim').attr('checked',true);
			if(dados[6] != 't') 	$('#picpregaonao').attr('checked',true);
			$('#novoPeriodo').click();
	    	$('#divPeriodo').show();
	    	$('#novoItem').show();
	    	$('#btnVoltarLista').show();
	    	carregarLista(picid);
	    	carregarListaPeriodo();
	   	}
	 });
}

function excluirItem( picid ){
	if( confirm('realmente deseja excluir este item?') ){
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=excluirItem&picid="+picid,
		   	dataType: 'json',
		   	success: function(msg){
		    	alert('Item excluido.');
		    	$('#novoItem').click();
		    	carregarLista();
		    	carregarListaPeriodo();
		   	}
		 });
	}
}

function onOffCampo( campo ) {
	var div_on  = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input   = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function checa100( obj ){

	if( obj.value.length > 2 ){
		obj.value = '100';
	}
}

function travarBotoesItem()
{
	$("[name=picdescricao],[name=picdetalhe],[id=ptsid],[name=pictipodespesa],[name=umiid],[id=salvarItem],[id=novoItem]").attr("disabled","disabled");
}

function liberarBotoesItem()
{
	$("[name=picdescricao],[name=picdetalhe],[id=ptsid],[name=pictipodespesa],[name=umiid],[id=salvarItem],[id=novoItem]").attr("disabled","");
}

$(document).ready(function() {

	$('#salvarItem').click(function() {
		if($('#picdescricao').val()==''){
			alert('Campo obrigatório');
			$('#picdescricao').focus();
			return false;
		}
		if($('#picdetalhe').val()==''){
			alert('Campo obrigatório');
			$('#picdetalhe').focus();
			return false;
		}
		var ptsid = document.getElementById('ptsid');
		selectAllOptions( ptsid );
		if($('#ptsid option:first').val()==''){
			if($('#ptsid_campo_off').css('display')!='none'){
		    	$('#ptsid_campo_off').click();
		    }
			alert('Campo obrigatório');
			$('#ptsid').focus();
			return false
		}
		if($('#ptsid').val()==''){
			alert('Campo obrigatório');
			$('#ptsid').focus();
			return false;
		}
		if($('#umiid').val()==''){
			alert('Campo obrigatório');
			$('#umiid').focus();
			return false;
		}

		if($('#existeitememenda').val()=='S' && document.getElementById('picemendasnao').checked == true){
			alert('Não é possivel desvincular esse item ao emenda.');
			$('#existeitememenda').focus();
			return false;
		}

		var dados = $('#formItem').serialize();

		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=salvarItem&"+dados,
		   	dataType: 'json',
		   	success: function(msg){

		    	$('#picid').val(msg.picid);
		    	$('#novoPeriodo').click();
		    	$('#divPeriodo').show();
		    	$('#novoItem').show();
		    	$('#btnVoltarLista').show();
		    	carregarLista();
		    	carregarListaPeriodo();
		    	alert('Item Cadastrado com Sucesso!');
		    	window.location.href = window.location;
		   	}
		 });
	});

	$('#salvarPeriodo').click(function() {

		var pregaoBo = true;
		var pregao   = '';
		$('input[name="pregao"]').each(function(){
			if( $(this).attr('checked') ){
				pregaoBo = false;
				pregao   = $(this).val();
			}
		});

		if(pregaoBo){
			alert('Campo "Pregão" obrigatório.');
			return false;
		}

		if( pregao == '1' ){
			$('input[name="pregaopublicado"]').each(function(){
				if( $(this).attr('checked') ){
					pregaopublicado   = $(this).val();
				}
			});
			if( pregaopublicado == '1' ){
				if($('#ptpid').val()==''){
					alert('Campo obrigatório');
					$('#ptpid').focus();
					return false;
				}
			}
			if( $('input[name="pregaopublicado"]').val() == '' ){
				alert('Campo obrigatório');
				$('input[name="pregaopublicado"]').focus();
				return false;
			}
		}else {
			if($('#dicvalor').val()==''){
				alert('Campo obrigatório');
				$('#dicvalor').focus();
				return false;
			}
			if($('#dicpercentual').val()==''){
				alert('Campo obrigatório');
				$('#dicpercentual').focus();
				return false;
			}
		}

		if($('#dicdatainicial').val()==''){
			alert('Campo obrigatório');
			$('#dicdatainicial').focus();
			return false;
		}
		if($('#dicdatafinal').val()==''){
			alert('Campo obrigatório');
			$('#dicdatafinal').focus();
			return false;
		}

		var obDt = new Data();
		if( !obDt.comparaData($('#dicdatainicial').val(), $('#dicdatafinal').val(), '<=') ){
			alert('A data inicial deve ser MENOR ou IGUAL que a data final.');
			$('#dicdatafinal').focus();
			return false;
		}
		var estuf = document.getElementById('estuf');
		selectAllOptions( estuf );
		if($('#estuf option:first').val()=='' || $('#estuf option').length == 0){
			if($('#estuf_campo_off').css('display')!='none'){
		    	$('#estuf_campo_off').click();
		    }
			alert('Campo obrigatório');
			$('#estuf').focus();
			return false
		}

		var dados = $('#formPeriodo').serialize();
		var picid = $('#picid').val();
		$.ajax({
			type: "POST",
			url: window.location,
		   	data: "req=verificaPregaoEstadoData&picid="+picid+"&"+dados,
		   	dataType: 'html',
		   	success: function(retorno){
		    	if($.trim(retorno) == "S"){
		    		var dados = $('#formPeriodo').serialize();
					var picid = $('#picid').val();
					$.ajax({
						type: "POST",
						url: window.location,
					   	data: "req=salvarPeriodo&picid="+picid+"&"+dados,
					   	dataType: 'html',
					   	success: function(msg){
					   		if($.trim(msg) == "S"){
					   			alert("Operação realizada com sucesso!");
					   			$('#dicid').val(msg);
						    	$('#novoPeriodo').click();
						    	$('#divPeriodo').show();
						    	carregarLista(picid);
						    	carregarListaPeriodo();
						    	liberarBotoesItem();
					   		}else{
					   			alert("Não foi possível realizar a operação!");
					   			return false;
					   		}
					   	}
					 });
		    	} else {
					alert("Não é permitido cadastrar detalhamentos com períodos concomitantes no estado!");
		    		return false;
		    	}
		   	}
		 });
	});

	$('input[name = "pregao"]').click(function() {
		if( $(this).val() == '1' ){
			$('#trPregaoPublicado').show();
//			$('#trPregao').show();
			$('#dicpercentual').val('');
			$('#dicdatainicial').val('');
			$('#dicdatafinal').val('');
			$('#trPercentual').hide();
		}else{
			$('#ptpid').val('');
			$('#ptpnumeroata').val('');
			$('#trPregaoPublicado').hide();
			$('#trPregao').hide();
			$('#trPercentual').show();
			$('#dicdatainicial').val('01/01/2011');
			$('#dicdatafinal').val('31/12/2014');
		}
	});

	$('input[name = "pregaopublicado"]').click(function() {
		if( $(this).val() == '1' ){
			$('#trPregao').show();
			$('#dicpercentual').val('');
			$('#dicdatainicial').val('');
			$('#dicdatafinal').val('');
			$('#trPercentual').hide();
		}else{
			$('#ptpid').val('');
			$('#ptpnumeroata').val('');
			$('#trPregao').hide();
			$('#trPercentual').show();
			$('#dicdatainicial').val('01/01/2011');
			$('#dicdatafinal').val('31/12/2014');
		}
	});

	$('#novoItem').click(function() {
		$('#picdescricao').val('');
		$('#picdetalhe').val('');
		$('#pictipodespesa').val('');
		$('#umiid').val('');
    	$('#picid').val('');
    	$('#ptsid').find('option').remove();
    	$('#ptsid').append('<option value="">Duplo clique para selecionar da lista</option>');
    	$('#divPeriodo').hide();
    	$('#novoItem').hide();

    	location.href='?modulo=principal/cadastraItensComposicao&acao=A';

	});

	$('#btnVoltarLista').click(function() {
    	location.href='?modulo=principal/cadastraItensComposicao&acao=A';
	});

	$('#novoPeriodo').click(function() {
    	$('#dicid').val('');
		//$('#ptpid').val('');
		$('#dicvalor').val('');
		$('#dicpercentual').val('');
		$('#dicdatainicial').val('');
    	$('#dicdatafinal').val('');
    	$('#estuf').children().remove();
    	$('#estuf').append('<option value="">Duplo clique para selecionar da lista</option>');
    	$('#novoPeriodo').hide();
    	liberarBotoesItem();

	});

});

</script>
<?php
echo '<br/>';
monta_titulo( $titulo_modulo, '' );
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<form method="post" name="formItem" id="formItem">
	<input type="hidden" id="picid" name="picid" value="" />
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" valign="bottom">Nome</td>
			<td>
				<?php
					$picdescricao = $_POST['picdescricao'];
					echo campo_texto( 'picdescricao', 'S', 'S', '', 40, 200, '', '','','','','id="picdescricao"' );
				?>
				<input type="button" value="Pesquisar" onclick="pesqItem()">
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Especificação técnica</td>
			<td>
				<?php
					$picdetalhe = $_POST['picdetalhe'];
					echo  campo_textarea( 'picdetalhe', 'S', 'S', $label, 100, $rows, $max, $funcao = '', $acao = 0, $txtdica = '', $tab = false, $title = NULL, $picdetalhe);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Tipo de Subação</td>
			<td>
			<?php
				// Tipo de Subação
				$sql = "SELECT
							pts.ptsid as codigo,
							pts.ptsdescricao || ' - ' || fe.frmdsc as descricao
						FROM
							par.propostatiposubacao pts
						INNER JOIN par.formaexecucao fe ON fe.frmid = pts.frmid
						where
							pts.ptsstatus = 'A'
						order by
							pts.ptsdescricao";
				$stSqlCarregados = "";
				combo_popup( "ptsid", $sql, "Tipo de Subação", "192x400", 0, array(), "", "S", false, false, 5, 400 );
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Categoria de despesa</td>
			<td>
				<?php
					$pictipodespesa = $_POST['pictipodespesa'];
					$sql = "(SELECT
								'1' as codigo,
								'Custeio' as descricao)
							UNION ALL
							(SELECT
								'2' as codigo,
								'Capital' as descricao)";
					$db->monta_combo('pictipodespesa',$sql,'S','Selecione...',$acao,$opc,'','200','S', 'pictipodespesa', $return = false, $pictipodespesa, $title= null);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Unidade de Medida</td>
			<td>
				<?php
					$umiid = $_POST['umiid'];
					$sql = "SELECT
								umiid as codigo,
								umidescricao as descricao
							FROM
								par.unidademedidadetalhamentoitem
							WHERE
								umistatus = 'A'
							ORDER BY
								umidescricao";
					$db->monta_combo('umiid',$sql,'S','Selecione...',$acao,$opc,'','200','S', 'umiid', $return = false, $umiid, $title= null);
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">Pertence tambem ao emendas?</td>
			<td>
				<input type="radio" value="S" id="picemendassim" name="picemendas" <? if($_REQUEST["picemendas"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="picemendasnao" name="picemendas" <? if($_REQUEST["picemendas"] == "N") { echo "checked"; } ?> /> Não
				<input type="hidden" id="existeitememenda" name="existeitememenda" value="" />
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom">É um item de pregão?</td>
			<td>
				<input type="radio" value="TRUE" id="picpregaosim" name="picpregao" <? if($_REQUEST["picpregao"] == "t") { echo "checked"; } ?> /> Sim
				<input type="radio" value="FALSE" id="picpregaonao" name="picpregao" <? if($_REQUEST["picpregao"] != "t") { echo "checked"; } ?> /> Não
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="bottom"></td>
			<td class="SubTituloDireita" align="left" style="text-align:left">
	         	<input type="button" id="salvarItem" value="Gravar" />
	         	<input type="button" id="novoItem"   value="Inserir Novo" style="display: none"/>
	         	<input type="button" id="btnVoltarLista"   value="Voltar para lista" style="display: none"/>

			</td>
		</tr>
	</table>
</form>
<form method="post" name="formPeriodo" id="formPeriodo">
	<div id="divPeriodo" style="display:none">
		<input type="hidden" id="dicid" name="dicid" value="" />
		<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td class="SubTituloDireita" colspan="2" align="center" style="text-align:center">
		         	<strong>
		         		Detalhamento do item
		         	</strong>
				</td>
		     </tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Pregão</td>
				<td>
					<?php
						$pregao = $_POST['pregao'];
						$sql = "(SELECT
									'1' as codigo,
									'Sim' as descricao)
								UNION ALL
								(SELECT
									'0' as codigo,
									'Não' as descricao)";
						$db->monta_radio('pregao',$sql,'S',$pregao);
					?>
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Data:&nbsp;
					<?php
//						echo campo_data2('','N','S',"","","","", $dicdatainicial,"","", '' ).'&nbsp;&nbsp;&nbsp;&nbsp;Comentário:&nbsp;';
//						echo campo_texto('', 'N', 'S', '', 80, 150, '', '','','','','');
					?>
				</td>
			</tr>
			<tr id="trPregaoPublicado" style="display: none">
				<td class="SubTituloDireita" valign="bottom">Pregão Publicado</td>
				<td>
					<?php
						$pregaopublicado = $_POST['pregaopublicado'];
						/*$sql = "(SELECT
									'1' as codigo,
									'Sim' as descricao)
								UNION ALL
								(SELECT
									'0' as codigo,
									'Não' as descricao)";
						$db->monta_radio('pregaopublicado',$sql,'S',$pregaopublicado);
						*/
						if( $pregaopublicado == 1 ){
							$ckSim = 'checked=checked';
						}elseif( $pregaopublicado == 0 ){
							$ckNao = 'checked=checked';
						}
					?>
					<input type="radio" name="pregaopublicado" <?=$ckSim ?> value="1"> Sim
					<input type="radio" name="pregaopublicado" <?=$ckNao ?> value="0"> Não
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					<?php
//						echo campo_data2('','N','S',"","","","", $dicdatainicial,"","", '' ).'&nbsp;&nbsp;&nbsp;&nbsp;Comentário:&nbsp;';
//						echo campo_texto('', 'N', 'S', '', 80, 150, '', '','','','','');
					?>
				</td>
			</tr>
			<tr id="trPregao" style="display: none">
				<td class="SubTituloDireita" valign="bottom">Ata</td>
				<td>
					<input type="hidden" name="ptpid" id="ptpid"> <input type="text" class="disabled" name="ptpnumeroata" id="ptpnumeroata" readonly> <input type="button" value="Buscar" onclick="window.open('par.php?modulo=principal/cadastraItensComposicao&acao=A&req=telaTipoPregao','Observações','scrollbars=yes,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');">
					<?php
					?>
				</td>
			</tr>
			<tr id="trPercentual">
				<td class="SubTituloDireita" valign="bottom">Percentual</td>
				<td>
					<?php
						$dicpercentual = $_POST['dicpercentual'];
						echo campo_texto( 'dicpercentual', 'S', 'S', '', 5, 3, '###', '','','','','id="dicpercentual"','','','checa100(this)' ).' %';
					?>
				</td>
			</tr>
			<tr id="trValor">
				<td class="SubTituloDireita" valign="bottom">Valor de Referência</td>
				<td>
					<?php
						$dicvalor = $_POST['dicvalor'];
						echo campo_texto( 'dicvalor', 'S', 'S', '', 25, 200, '[.###],##', '','','','','id="dicvalor"' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Periodo Inicial</td>
				<td>
					<?php
						$dicdatainicial = $_POST['dicdatainicial'];
						echo campo_data2('dicdatainicial','S','S',$label,$formata,$txtdica='',$txtOnBlur='', $dicdatainicial, $txtOnChange='', $classe='', 'dicdatainicial' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom">Periodo Final</td>
				<td>
					<?php
						$dicdatafinal = $_POST['dicdatafinal'];
						echo campo_data2('dicdatafinal','S','S',$label,$formata,$txtdica='',$txtOnBlur='', $dicdatafinal, $txtOnChange='', $classe='', 'dicdatafinal' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita">Estado</td>
				<td>
				<?php
					// UF
					$ufSql = " SELECT
									est.estuf AS codigo,
									est.estuf||' - '||est.estdescricao AS descricao
								FROM
									territorios.estado est
								/*INNER JOIN
									obras.preobra po ON po.estuf = est.estuf*/
								WHERE
									1=1
								 	--po.prestatus = 'A'
								ORDER BY
									estdescricao ";
					$stSqlCarregados = "";
					combo_popup( "estuf", $ufSql, "Estado", "192x400", 0, array(), "", "S", false, false, 5, 400 );
				?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom"></td>
				<td class="SubTituloDireita" align="left" style="text-align:left">
		         	<input type="button" id="salvarPeriodo" value="Gravar Detalhamento"/>
		         	<input type="button" id="novoPeriodo"   value="Novo Detalhamento"   style="display: none"/>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="bottom"></td>
				<td>
					<div id="listaPeriodo">
					<?php

					listaPeriodos( $picid );

					?>
					</div>
				</td>
			</tr>
		</table>
	</div>
</form>
<div id="lista">
<?php

listaItens();

if( $_GET['carregaitem'] ){
	echo "<script>carregarItem(".$_GET['carregaitem'].");</script>";
}
?>
</div>