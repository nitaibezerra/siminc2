<?

//include APPRAIZ . 'conjur/classes/WorkflowConjur.php';

if($_REQUEST['req'] == 'deletaAnexo'){
	

	
	if($_REQUEST['prvid']){

		$sql = "delete from conjur.processosvinculados where prvid = ".$_REQUEST['prvid'];
		$db->executar($sql);
		$db->commit();
		
		$sql = "select 
					count(*) 
				from 
					conjur.processosvinculados 
				where 
					prcid = {$_SESSION['conjur_var']['prcid']}";
		
		$rsTotal = $db->pegaUm($sql);
		
		if($rsTotal == 0 || $rsTotal == '0'){
			
			$docid = pegarDocid($_SESSION['conjur_var']['prcid']);
		
			if($docid){
				
				$sql = "update workflow.documento set esdid = 374 where docid = {$docid}";
				$db->executar($sql);
				$db->commit();
			}
		}
		
		die('true');
	}
	die('false');
}


if($_REQUEST['req'] == 'ACsidoc'){
	ob_clean();
	$sql = "SELECT prcnumsidoc FROM conjur.processoconjur"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($key), $q) !== false) {
			$prcnumsidoc = trim($value['prcnumsidoc']);
			echo "{$prcnumsidoc}|{$prcnumsidoc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprcnomeinteressado'){
	ob_clean();
	$sql = "SELECT prcnomeinteressado FROM conjur.processoconjur"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prcnomeinteressado']), $q) !== false) {
			$prcnomeinteressado = $value['prcnomeinteressado'];
			echo "{$prcnomeinteressado}|{$prcnomeinteressado}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACtasdsc'){
	ob_clean();
	$sql = "SELECT tasdsc FROM conjur.tipoassunto WHERE tasstatus = 'A' ORDER BY tasdsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['tasdsc']), $q) !== false) {
			$tasdsc = $value['tasdsc'];
			echo "{$tasdsc}|{$tasdsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['req'] == 'ACprodsc'){
	ob_clean();
	$sql = "SELECT prodsc FROM conjur.procedencia WHERE unpid = '".$_REQUEST['unpid']."' AND prodtstatus = 'A' ORDER BY	prodsc"; 
	$arDados = $db->carregar($sql);
	$arDados = ($arDados) ? $arDados : array();
	 
	$q = strtolower($_GET["q"]);
	if ($q === '') return;
	foreach ($arDados as $key=>$value){
		if (strpos(strtolower($value['prodsc']), $q) !== false) {
			$prodsc = $value['prodsc'];
			echo "{$prodsc}|{$prodsc}\n";			
		}
	}
	
	die;

}

if($_REQUEST['requisicaoAjax'] != '') {
//	header('content-type: text/html; charset=ISO-8859-1');
	$_REQUEST['requisicaoAjax']($_REQUEST);
	die();
}

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
}

$prcid = $_REQUEST['prcid'];

include APPRAIZ . 'includes/workflow.php';
include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

/* Gravando a variável de sessão */
if($_REQUEST['prcid'])
	$_SESSION['conjur_var']['prcid'] = $_REQUEST['prcid'];
	
/* Tratamento dos documentos */
$docid = pegarDocid($_SESSION['conjur_var']['prcid']);
if(!$docid) {
	$docid = criarDocumento($_SESSION['conjur_var']['prcid']);
}

// verifica se muda a cor da aba de documento
$boMudaCorAba = verificaSeMudaCor($_SESSION['conjur_var']['prcid']);


// Verificando se projeto está aguardando resposta externa e tem data de prazo prevista

/* Carregando o projeto CONJUR, testando os possíveis erros */
if($_SESSION['conjur_var']['prcid']) {
	$sql = "SELECT 
				proj.*, 
				had.advid as advogado,
				pro.unpid,
				pro.prodsc,
				esd.esdid,
				tas.tasdsc,
				espdtrespexterna as prazo,
				espnumdiasrespexterna as prazo_old,
				proj.tacid  
			FROM conjur.processoconjur proj
			LEFT JOIN (SELECT max(hadid), advid, prcid, dtatribuicao 
					   FROM conjur.historicoadvogados 
					   GROUP BY hadid,advid, prcid, dtatribuicao
					   ORDER BY hadid DESC ) had ON had.prcid = proj.prcid
			LEFT JOIN conjur.tipoassunto 	   tas ON tas.tasid = proj.tasid
			LEFT JOIN conjur.procedencia 	   pro ON pro.proid = proj.proid
			LEFT JOIN conjur.estruturaprocesso esp ON esp.prcid = proj.prcid
			LEFT JOIN workflow.documento 	   doc ON doc.docid = esp.docid
			LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			WHERE proj.prcid='".$_SESSION['conjur_var']['prcid']."'";
	
	$processoconjur = $db->pegaLinha($sql);
	
	if($processoconjur) {
		if( $processoconjur['esdid'] == WF_AGUARDANDO_RESPOSTA_EXTERNA ){
			if( $processoconjur['prazo'] == '' && $processoconjur['prazo_old'] == '' ){
				direcionar('?modulo=principal/prazoRespostaExterna&acao=C','Processo não possui prazo para resposta externa.');
			}
			$prazo = $processoconjur['prazo'];
		}
		
		$cooid = $processoconjur['cooid'];
		$advid = $processoconjur['advogado'];
		$prcnumsidoc = $processoconjur['prcnumsidoc'];
		$prcnumeroprocjudicial = $processoconjur['prcnumeroprocjudicial'];
		$prcnomeinteressado = $processoconjur['prcnomeinteressado'];
		$tprid = $processoconjur['tprid'];
		$_SESSION['conjur_var']['tprid'] = $tprid;
		$prcdtentrada 	= $processoconjur['prcdtentrada'];
		$proid 		  	= $processoconjur['proid'];
		$prodsc 	  	= $processoconjur['prodsc'];
		$unpid 	      	= $processoconjur['unpid'];
		$tasid 		  	= $processoconjur['tasid'];
		$tasdsc			= $processoconjur['tasdsc'];
		$prcdesc 	  	= $processoconjur['prcdesc'];
		$tipid 		    = $processoconjur['tipid'];
		$tacid 		    = $processoconjur['tacid'];
		$prcprioritario = $processoconjur['prcprioritario'];
		$prcnumeroprocjudantigo = $processoconjur['prcnumeroprocjudantigo'];
        $prcdtprazoadv          = $processoconjur['prcdtprazoadv'];
	} else {
		direcionar('?modulo=inicio&acao=C','Não existe o projeto solicitado. Entre em contato com a equipe técnica.');
	}
} else {
	direcionar('?modulo=inicio&acao=C','Ocorreu um problema na variável de sessão, caso o erro persista entre em contato com a equipe técnica.');
}

$estado_documento = wf_pegarEstadoAtual( $docid );
$dadosestruturaprocessoid = $db->pegaUm("SELECT espid FROM conjur.estruturaprocesso WHERE prcid = '".$_SESSION['conjur_var']['prcid']."'");
/* FIM - Tratamento dos documentos */

/* Tratamento de segurança */
$permissoes = verificaPerfilConjur($estado_documento['esdid'],$cooid);
$perfilid = $perfilid == '' ? $_SESSION['conjur']['perfilid'] : $perfilid;
/* FIM - Tratamento de segurança */
$_SESSION['conjur']['coiid'] = $cooid;
echo "<br/>";
echo montarAbasArray(monta_abas_processo($_SESSION['conjur_var']['tprid']),$_SERVER['REQUEST_URI']);
monta_cabecalho_conjur($_SESSION['conjur_var']['prcid']);

// se não for administrador, não pode alterar alguns campos
$boAdmin = false;
if( possuiPerfil(PRF_ADMINISTRADOR) || possuiPerfil(PRF_TECNICO_ADM) )
{
	$boAdmin = true;
}

?>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="./js/ajax.js"></script>
<script language="javascript" type="text/javascript" src="./js/conjur.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type='text/javascript' src='../includes/jquery-autocomplete/jquery.autocomplete.js'></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-autocomplete/jquery.autocomplete.css" />
<script>

	jQuery.noConflict();

	jQuery(document).ready(function(){
		// Serve para mudar a cor da aba documentos, quando existe número gerado e não tem arquivo anexado
		var boMudaCorAba = '<?php echo $boMudaCorAba;?>';
		if(!boMudaCorAba){
			if(jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().text() != 'Documentos'){
				jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			} else {
				jQuery('.tbl_conteudo').find('table:first').find('table:first').find('td:first').next().next().next().next().next().next().next().next().next().css('color','red').css('font-weight','bold');
			}
			
		}

		jQuery('#prodsc').removeClass('normal ac_input');
		jQuery('#prodsc').addClass('disabled ac_input');
		
		jQuery('#prcnumsidoc').autocomplete("conjur.php?modulo=principal/editarprocesso&acao=A&req=ACsidoc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#prcnomeinteressado').autocomplete("conjur.php?modulo=principal/editarprocesso&acao=A&req=ACprcnomeinteressado", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});
		
		jQuery('#tasdsc').autocomplete("conjur.php?modulo=principal/editarprocesso&acao=A&req=ACtasdsc", {
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#prodsc').autocomplete("conjur.php?modulo=principal/editarprocesso&acao=A&req=ACprodsc", {
			extraParams: 
			{
		       unpid: function() { return jQuery("#unpid").val(); }
			},
		  	cacheLength:10,
			width: 440,
			scrollHeight: 220,
			selectFirst: true,
			autoFill: true
		});

		jQuery('#unpid').change(function(){
			if(jQuery(this).val()!='')
			{
				if(jQuery('#prodsc').hasClass('disabled'))
				{
					jQuery('#prodsc').removeClass('disabled ac_input');
					jQuery('#prodsc').addClass('normal ac_input');
				}
			}
			else
			{
				jQuery('#prodsc').removeClass('normal ac_input');
				jQuery('#prodsc').addClass('disabled ac_input');
				
			}
		});

		jQuery('#advid').live('change',function(){
			if( jQuery(this).val() == '' ){
				return false;
			}
			if(confirm('Esta ação resultará no encaminhamento desse processo ao advogado escolhido.\nDeseja prosseguir?')){
                

                //jQuery("#div_dialog_tramit_adv").html(html);
                jQuery("#div_dialog_tramit_adv").show();
                jQuery("#div_dialog_tramit_adv").dialog({
                    resizable: true,
                    width: 700,
                    modal: true,
                    show: { effect: 'drop', direction: "up" },
                    buttons: {
                        "Fechar": function() {
                            jQuery(this).dialog("close");
                        },
                        "Tramitar": function() {
                            //alert(jQuery('#haddtprazoadv_t').val());
                            tramitaAdvogado( jQuery('#advid').val(),  jQuery('#haddtprazoadv_t').val());
                        }
                    }
                });

//				tramitaAdvogado( jQuery(this).val() );
//				window.location = window.location;
			}
		});
	});

	function tramitaAdvogado(advid, haddtprazoadv)
	{
        console.log('1');
        haddtprazoadv = typeof haddtprazoadv !== 'undefined' ? haddtprazoadv : '';
        console.log('2');

		if( advid == '' ){
            console.log('3');
			return false;
		}
        console.log('4');

        if (haddtprazoadv == '') {
            console.log('5');
            alert("Prazo do Advogado não pode ficar em branco.");
            return false;
//            jQuery.ajax({
//                type: "POST",
//                url: window.location,
//                data: "requisicaoAjax=encaminhaParaAdvogado&advid=" + advid,
//                success: function (data) {
////				alert(data);
//                    //window.location = window.location;
//                    //location.href = "conjur.php?modulo=principal/editarprocesso&acao=A&prcid=<?//=$_SESSION['conjur_var']['prcid']?>//";
//                }
//            });
        } else {
            console.log('6');
            jQuery.ajax({
                type: "POST",
                url: window.location,
                data: "requisicaoAjax=encaminhaParaAdvogado&advid=" + advid + "&haddtprazoadv=" +haddtprazoadv,
                success: function (data) {
				    //alert(data);
                    //window.location = window.location;
                    location.href = "conjur.php?modulo=principal/editarprocesso&acao=A&prcid=<?=$_SESSION['conjur_var']['prcid']?>";
                }
            });
        }
	}
	

	function verprocesso( prcid ){
		window.location = '?modulo=principal/editarprocesso&acao=A&prcid=' + prcid;
	}
	function exibirOcultarNumeracaoUnica(value) {
		var isJudicial = value==2; // 2, conforme ID na base de dados 
		if( isJudicial ) {
			document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = "visible";
		} else {
			document.getElementById('linhaNumeracaoUnicaJudicial').style.visibility = "collapse";
			//document.formulario.prcnumeroprocjudicial.value = "";			
		}
	}	
	function conjurJs_digitoVerificador() {
		var campoNumeroProcessoSidoc    = document.getElementById('prcnumsidoc');
		var campoNumeroProcessoSidocbkp = document.getElementById('prcnumsidoc2').value;
		
		if(!conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc)){
			document.getElementById('prcnumsidoc').value = campoNumeroProcessoSidocbkp;
			return false;
		} 
	}
	
	function exibeAdvogados(value){
		if(!value){
			jQuery("#advid").val("");
			jQuery("#advid").attr("disabled","disabled");
		}else{
			jQuery("#advid").attr("disabled","disabled");
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=carregarAdvogados&cooid=" + value,
				success: function(data) {
//					alert(data);
					jQuery("#combo_advogados").html(data);
			  }
			});
		}
	}
	function atualizaAdvogado(advid)
	{
		if(!advid){
			return false;
		}
		<?php if($docid): ?>
		jQuery.ajax({
			type: "POST",
			url: window.location,
			data: "requisicaoAjax=carregarWorkflow&docid=<?=$docid?>&advid=" + advid,
			success: function(data) {
				jQuery("#barra_workflow").html(data);
			}
		});
		<?php endif; ?>
		window.location = window.location;
	}
	function atualizaCoordenacao(cooid)
	{
		if(!cooid){
			cooidv = "null";
		}
			<?php if($docid): ?>
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "requisicaoAjax=carregarWorkflow2&docid=<?=$docid?>&cooid=" + cooid,
				success: function(data) {
					jQuery("#barra_workflow").html(data);
				}
			});
			<?php endif; ?>
	}

</script>
<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
    <tr>
        <td style="width: 95%;">
            <form name="formulario" id="formulario" method="post" action="">
                <input type="hidden" name="requisicao" value="atualizarprocessoconjur">
                <table align="center" border="0" cellpadding="5" cellspacing="1" style="width: 100%;">
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2"><b>Cadastro de Processo</b></td>
                        <td rowspan="10" valign="top" id="barra_workflow" >
                            <?
                            if($cooid) {
                                $sqlAdvogado = "SELECT adv.advid as codigo, ent.entnome as descricao
							FROM conjur.advogados adv
							INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
							WHERE
								adv.coonid='".$cooid."' AND
								ent.entstatus = 'A' AND adv.advstatus = 'A'
						   	ORDER BY ent.entnome";
                                $avds = $db->carregar($sqlAdvogado);
                                $avds = is_array($avds) ? $avds : array();
                                $avdids = $db->carregarColuna($sqlAdvogado,0);
                                if( !in_array($advid,$avdids) ){
                                    $sql = "SELECT
							adv.advid as codigo,
							ent.entnome as descricao
						FROM
							conjur.advogados adv
						INNER JOIN entidade.entidade ent ON ent.entid = adv.entid
						WHERE
							advid = (SELECT advid
									 FROM conjur.historicoadvogados
									 WHERE hadid = (SELECT max(hadid)
									 				FROM conjur.historicoadvogados
									 				WHERE prcid = ".$_SESSION['conjur_var']['prcid'].") )";

                                    $advnovo = $db->pegaLinha($sql);
                                    array_push($avds,$advnovo);
                                }

                                $advexiste = false;

                                if($avds[0]) {

                                    foreach($avds as $adv) {

                                        if($adv['codigo']==$advid) {
                                            $advexiste = true;
                                        }

                                        $dadosAdvogado[] = $adv;
                                    }

                                    if(!$advexiste) {
                                        $db->executar("UPDATE conjur.processoconjur SET advid=NULL WHERE prcid='".$_SESSION['conjur_var']['prcid']."'");
                                        $db->commit();
                                    }

                                } else {

                                    $dadosAdvogado = array();
                                }

                            } else {
                                $dadosAdvogado = array();
                            }

                            //		/* Barra de estado atual e ações e Historico */
                            //		wf_desenhaBarraNavegacao( $docid, array( 'docid' => $docid, 'prcid' => $_SESSION['conjur_var']['prcid'], 'cooid' => $cooid, 'usar_acaoPossivel2' => true ) );
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width:20%;"><b>Selecione a Unidade :</b></td>
                        <td>
                            <?
                            //			//Restrição para Coord. por Perfil de Técnico Coordenação.
                            //			if($perfilid == PRF_TECNICO && $cooid){
                            //				$sql = "SELECT coonid FROM conjur.usuarioresponsabilidade
                            //							WHERE rpustatus = 'A' AND usucpf = '{$_SESSION['usucpf']}'";
                            //				$coordenacao_id = $db->pegaUm($sql);
                            //				$where = $coordenacao_id ? $coordenacao_id .', ' : '';
                            //				$sqlCoordenacao = "SELECT coonid as codigo, coodsc as descricao
                            //							   FROM conjur.coordenacao
                            //							   WHERE coonid IN ( {$where} 4)
                            //							   ORDER BY	coodsc";
                            //			ver($cooid,$perfilid, PRF_TECNICO);
                            //			}
                            //			else{
                            $sqlCoordenacao = "SELECT coonid as codigo, coodsc as descricao
							   FROM conjur.coordenacao
							   ORDER BY	coodsc";
                            //			}
                            $db->monta_combo('cooid', $sqlCoordenacao, 'N', 'Selecione...', '', '', '', 300, 'N','cooid');
                            ?>
                        </td>
                    </tr>
                    <?php $esdid = pegaEstadoWorkFlow($docid);
                    if(($permissoes['selecionaradvogado'] && $esdid==WF_EM_ANALISE_COORDENADOR_GERAL && conjur_regraB( $cooid ))): ?>
                        <tr>
                            <td class="SubTituloDireita" style="width:20%;">
                                <b>Coordenador Geral :</b>
                            </td>
                            <td id="combo_advogados">
                                <?php
                                $sql = "select entnome from conjur.coordenacao coo
					join conjur.advogados adv on adv.advid = coo.advid
					join entidade.entidade ent on ent.entid = adv.entid
					where coo.coonid = {$cooid}";

                                $coordenador = $db->pegaUm($sql);

                                echo $coordenador;
                                ?>
                            </td>
                        </tr>
                    <?php endif;
                    if(($permissoes['selecionaradvogado'] &&( $esdid== WF_EM_ANALISE_CHEFEDIVISAO) && conjur_regraB( $cooid ) )): ?>
                        <tr>
                            <td class="SubTituloDireita" style="width:20%;">
                                <b>Chefe de divisão :</b>
                            </td>
                            <td id="combo_advogados">
                                <?php
                                $sql = "select entnome from conjur.coordenacao coo
					join conjur.advogados adv on adv.advid = coo.advcd
					join entidade.entidade ent on ent.entid = adv.entid
					where coo.coonid = {$cooid}";

                                $coordenador = $db->pegaUm($sql);

                                echo $coordenador;
                                ?>
                            </td>
                        </tr>
                    <?php endif;
                    if(($permissoes['selecionaradvogado'] &&( $esdid== WF_EM_ANALISE_COORDENADOR) && conjur_regraB( $cooid ) )): ?>
                        <tr>
                            <td class="SubTituloDireita" style="width:20%;">
                                <b>Coordenador :</b>
                            </td>
                            <td id="combo_advogados">
                                <?php
                                $sql = "select entnome from conjur.coordenacao coo
					join conjur.advogados adv on adv.advid = coo.advcoord
					join entidade.entidade ent on ent.entid = adv.entid
					where coo.coonid = {$cooid}";

                                $coordenador = $db->pegaUm($sql);

                                echo $coordenador;
                                ?>
                            </td>
                        </tr>
                    <?php endif; ?>
                    <tr>
                        <td class="SubTituloDireita" style="width:20%;">
                            <b>Selecione advogado :</b>
                        </td>
                        <td id="combo_advogados">
                            <?php
                            $db->monta_combo( 'advid', $dadosAdvogado, 'S', 'Selecione...', '', '', '', 300, 'N', 'advid', '', $advid );
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tipo de Processo :</td>
                        <td><?
                            $sqlTipoProcesso = "SELECT tprid as codigo, tprdsc as descricao
							FROM conjur.tipoprocesso
							WHERE tprstatus = 'A'
							ORDER BY tprdsc";
                            $db->monta_combo('tprid', $sqlTipoProcesso, (($permissoes['gravar'] && $boAdmin)?'S':'N'), 'Selecione...', 'exibirOcultarNumeracaoUnica', '', '', 150, 'S', 'tprid');
                            ?></td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita" style="width:20%;">Número do Processo SIDOC :</td>
                        <td><?
                            $pcjnumerosidoc = $_REQUEST['prcnumsidoc'];
                            echo campo_texto('prcnumsidoc', 'S', (($boAdmin)?'S':'N'), '', 35, 25, '', '', 'left', '', 0, 'id="prcnumsidoc"','','','');
                            ?>
                            <input type="hidden" name="prcnumsidoc2" id="prcnumsidoc2" value="<?=$prcnumsidoc ?>"/>
                        </td>

                    </tr>

                    <tr id="linhaNumeracaoUnicaJudicial" style="visibility: <?= ($tprid=='2' ? 'visible' : 'collapse' ) ?>;">
                        <td class="SubTituloDireita" style="width:20%;">Numeração Única Judicial :</td>
                        <td><? echo campo_texto('prcnumeroprocjudicial', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 30, 25, '#######-##.####.#.##.####', '', 'right', '', 0, ' style="text-align:right;" ','',$prcnumeroprocjudicial); ?></td>
                    </tr>

                    <tr style="visibility: visible;">
                        <td class="SubTituloDireita" style="width:20%;">Numeração Judicial Antiga :</td>
                        <td><? echo campo_texto('prcnumeroprocjudantigo', 'S', (($boAdmin)?'S':'N'), '', 30, 25, '#####-####-###-##-##-##', '', 'right', '', 0, ' style="text-align:right;" '); ?></td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Interessado :</td>
                        <td><? echo campo_texto('prcnomeinteressado', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 50, 255, '', '', 'left', '', 0, 'prcnomeinteressado'); ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Data de Entrada do Processo :</td>
                        <td><? echo campo_data('prcdtentrada', 'S', (($permissoes['gravar'] && $boAdmin)?'S':'N'), '', 'S'); ?></td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Tipo de Procedência :</td>
                        <td><?

                            //$unpid = $_REQUEST['unpid'];
                            $sqlTipoProcedencia = "SELECT unpid as codigo,	unpdsc as descricao
						   FROM conjur.unidadeprocedencia
						   WHERE undpstatus = 'A'
						   ORDER BY	unpdsc";
                            $db->monta_combo('unpid', $sqlTipoProcedencia, (($permissoes['gravar'] && $boAdmin)?'S':'N'), 'Selecione...', '', '', '', 300, 'S','unpid');
                            ?></td>
                    </tr>

                    <tr>
                        <td align='right' class="SubTituloDireita">Procedência:</td>
                        <td>
                            <?
                            //			$prodsc = $_REQUEST['prodsc'];
                            echo campo_texto('prodsc', 'S', (($boAdmin)?'S':'N'), '', 50, 255, '', '', 'left', '', 0, 'id="prodsc"');
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Tema :</td>
                        <td>
                            <?
                            //			$tasdsc = $_REQUEST['tasdsc'];
                            echo campo_texto('tasdsc', 'N', 'S', '', 50, 255, '', '', 'left', '', 0, 'id="tasdsc"');
                            ?>
                        </td>
                    </tr>
                    <?php if( $tprid == 2 ): ?>
                        <tr>
                            <td class="SubTituloDireita">Tipo de Ação :</td>
                            <td>
                                <?
                                $sqlTipoAcao = "SELECT tacid as codigo, tacdsc as descricao
					    FROM conjur.tipoacao
					    ORDER BY tacdsc";
                                $db->monta_combo('tacid', $sqlTipoAcao, (($permissoes['gravar'])?'S':'N'), 'Selecione...', '', '', '', 300, 'N', 'tacid');
                                ?>
                            </td>
                        </tr>
                    <?php endif; ?>
                    <tr>
                        <td class="SubTituloDireita">Prioridade :</td>
                        <td><?
                            $sqlPrioridade = "SELECT tipid as codigo, tipdsc as descricao
					      FROM conjur.tipoprioridade
					      ORDER BY	tipdsc";
                            $db->monta_combo('tipid', $sqlPrioridade, (($permissoes['gravar'])?'S':'N'), 'Selecione...', '', '', '', 300, 'S', 'tipid');
                            ?></td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Relevante :</td>
                        <td>
                            <input type="radio" name="prcprioritario" id="prcprioritario" value="sim" <?php if($prcprioritario=='t'){ echo 'checked=\"checked\"'; } ?>/> Sim
                            <input type="radio" name="prcprioritario" id="prcprioritario" value="nao" <?php if($prcprioritario=='f'){ echo 'checked=\"checked\"'; } ?>/> Não
                        </td>
                    </tr>

                    <tr>
                        <td class="SubTituloDireita">Assunto :</td>
                        <td><? echo campo_textarea('prcdesc', 'N', (($permissoes['gravar'] && possuiPerfil(Array(PRF_ADMINISTRADOR,PRF_APOIO_PROTOCOLO,PRF_TECNICO_ADM)) )?'S':'N'), '', 100, 7, 500); ?></td>
                    </tr>
                    <?php
                    if( $processoconjur['esddsc'] == 'Aguardando Resposta Externa' ){
                        ?>
                        <tr>
                            <td class="SubTituloDireita">Prazo para resposta externa :</td>
                            <td><?= campo_texto('prazo', 'S','','',5,5,'','','left','',0,' id="prazo"'); ?></td>
                        </tr>
                    <?php
                    }
                    ?>
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2"><b>Partes no Processo</b></td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table id="tabela_interessado" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                <tr>
                                    <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nome</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo Pessoal</strong></td>
                                </tr>
                                <?
                                $sql = "SELECT * FROM conjur.interessadosprocesso as i
					LEFT JOIN entidade.entidade as e ON i.entid = e.entid
					WHERE i.prcid = ".$_SESSION['conjur_var']['prcid'];
                                $interessados = $db->carregar($sql);
                                if($interessados[0]) {
                                    foreach($interessados as $in) {
                                        if($permissoes['gravar']) {
                                            $acoes = "<input type=\"hidden\" value=\"".$in['entid']."\" id=\"entid_".$in['entid']."\" name=\"entid[]\"/>
								  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick=\"atualizaInteressado('linha_" . $in['entid']."');\"/>
								  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarlinhainteressado(this);\"/>";
                                        } else {
                                            $acoes = "&nbsp;";
                                        }
                                        echo "<tr>";
                                        echo "<td><center>".$acoes."</center></td>";
                                        echo "<td>".$in['entnome']."</td>";
                                        echo "<td>";
                                        echo ((strlen($in['entnumcpfcnpj']) == 11)?"Física":"Jurídica");
                                        echo "</td>";
                                    }
                                }
                                ?>
                            </table>
                        </td>
                    </tr>
                    <? if($permissoes['gravar']) {?>
                        <tr>
                            <td colspan="2">
                                <input type="button" value="Inserir Partes" style="cursor:pointer" onclick="abreJanelaCadastroInteressados();">
                            </td>
                        </tr>
                    <? } ?>
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2"><b>Expressão Chave</b></td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table id="tabela_expressao" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                <tr>
                                    <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Expressão Chave</strong></td>
                                </tr>
                                <? if($permissoes['gravar']) {?>
                                    <tr>
                                        <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><img style="cursor:pointer;" src="../imagens/gif_inclui.gif"  onclick="cadastrarExpressao();"></td>
                                        <td valign="top" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><input type="text" class="normal" id="expressao_chave" size="30" maxlength="20" value="<?=$_REQUEST["expressao_chave"]?>"></td>
                                    </tr>
                                <? } ?>
                                <?
                                $sql = "SELECT * FROM conjur.expressaochave as i
 					WHERE i.prcid = ".$_SESSION['conjur_var']['prcid'];
                                $pchave = $db->carregar($sql);
                                if($pchave[0]) {
                                    foreach($pchave as $in) {
                                        if($permissoes['gravar']) {
                                            $acoes = "<input type='hidden' name='expressaochave[]' value='".$in['excdsc']."'>
								  <img src=\"/imagens/alterar.gif\" style=\"cursor:pointer\" border=\"0\" title=\"Editar\" onclick='editarExpressao(this.parentNode.parentNode.parentNode.rowIndex);'/>
								  <img src=\"/imagens/excluir.gif\" style=\"cursor:pointer\"  border=\"0\" title=\"Excluir\" onclick=\"deletarExpressao(this.parentNode.parentNode.parentNode.rowIndex);\"/>";
                                        } else {
                                            $acoes = "&nbsp;";
                                        }
                                        echo "<tr>";
                                        echo "<td><center>".$acoes."</center></td>";
                                        echo "<td>".$in['excdsc']."</td>";
                                    }
                                }
                                ?>
                            </table>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Processo(s) Anexado(s)</td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <table id="tabela_vincprocessos" width="100%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                <tr>
                                    <td valign="top" align="center" class="title" style="width:80px; border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ações</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Nº do processo</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Tipo</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Interessado</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data Entrada</strong></td>
                                    <td valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;background-color: #E3E3E3;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Situação Conjur</strong></td>
                                </tr>
                                <?
                                $excluir = "'<center><img onclick=\"alert(\'Processo físico não está na CONJUR e/ou o usuário logado não está vinculado à unidade.\');\" src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'";
                                $arEstados = Array(WF_ENCERRADO,
                                    WF_AGUARDANDO_RESPOSTA_EXTERNA);
                                $coordUsu = pegaCoordUsu();
                                $cooid = pegaCoordProc( $_SESSION['conjur_var']['prcid'] );

                                //if( !in_array($esdid, $arEstados)&&(conjur_regraB( $cooid )||$db->testa_superuser()) ){
                                if( !in_array($esdid, $arEstados) && (possuiPerfil(PRF_GESTOR)||$db->testa_superuser()) ){
                                    $excluir = "'<center><img id=\"' || prvid || '\" onclick=\"deletarlinha(this,' || prvid || ');\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'";
                                }

                                $sql = "SELECT
						( CASE WHEN
							( 	select
									htddata
								from
									conjur.estruturaprocesso esppai
								LEFT JOIN
									workflow.documento docpai ON docpai.docid = esppai.docid
								LEFT JOIN
									workflow.historicodocumento hsd ON docpai.docid = hsd.docid
								where
									esppai.prcid = pv.prcid
								order by
									htddata desc
								limit
									1
							 ) > prvdtvinculacao
							 then '<center><img onclick=\"alert(\'O Processo não pode ser desvinculado devido a movimentação do Processo Anexador!\');\" src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'
							 else $excluir
						end)||
						(

						select
									htddata
								from
									conjur.estruturaprocesso esppai
								LEFT JOIN
									workflow.documento docpai ON docpai.docid = esppai.docid
								LEFT JOIN
									workflow.historicodocumento hsd ON docpai.docid = hsd.docid
								where
									esppai.prcid = pv.prcid
								order by
									htddata desc
								limit
									1

						)||' <br> '||prvdtvinculacao as img,
						'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>' as numero,
						tprdsc,
						prcnomeinteressado,
						to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada,
						esd.esddsc
					FROM
						conjur.processosvinculados as pv
					LEFT JOIN
						conjur.estruturaprocesso esppai ON pv.prcid = esppai.prcid
					LEFT JOIN
						conjur.processoconjur prc ON prc.prcid = pv.pro_prcid
					LEFT JOIN
						conjur.tipoprocesso tpr ON tpr.tprid = prc.tprid
					LEFT JOIN
						conjur.estruturaprocesso esp ON prc.prcid = esp.prcid
					LEFT JOIN
						workflow.documento doc ON doc.docid = esp.docid
					LEFT JOIN
						workflow.estadodocumento esd ON esd.esdid = doc.esdid
					WHERE
						pv.prcid = ".$_SESSION['conjur_var']['prcid'];

                                if( possuiPerfil(PRF_ADMINISTRADOR) || possuiPerfil(PRF_TECNICO_ADM) ){
                                    $sql = "SELECT
							'<center><img onclick=\"alert(\'Somente o Gestor pode exluir este anexo.\');\" src=\"/imagens/excluir_01.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'  as img,
							'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>' as numero,
							tprdsc,
							prcnomeinteressado,
							to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada,
							esd.esddsc
						FROM
							conjur.processosvinculados as pv
						LEFT JOIN
							conjur.estruturaprocesso esppai ON pv.prcid = esppai.prcid
						LEFT JOIN
							conjur.processoconjur prc ON prc.prcid = pv.pro_prcid
						LEFT JOIN
							conjur.tipoprocesso tpr ON tpr.tprid = prc.tprid
						LEFT JOIN
							conjur.estruturaprocesso esp ON prc.prcid = esp.prcid
						LEFT JOIN
							workflow.documento doc ON doc.docid = esp.docid
						LEFT JOIN
							workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE
							pv.prcid = ".$_SESSION['conjur_var']['prcid'];
                                }

                                if( possuiPerfil(PRF_SUPERUSUARIO) || possuiPerfil(PRF_GESTOR) ){
                                    $sql = "SELECT
							'<center><img id=\"' || prvid || '\" onclick=\"deletarlinha(this,' || prvid || ');\" src=\"/imagens/excluir.gif\" style=\"cursor:pointer\" border=\"0\"></center><input type=\"hidden\" name=\"pro_prcid['||prc.prcid||']\" value=\"sim\">'  as img,
							'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>' as numero,
							tprdsc,
							prcnomeinteressado,
							to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada,
							esd.esddsc
						FROM
							conjur.processosvinculados as pv
						LEFT JOIN
							conjur.estruturaprocesso esppai ON pv.prcid = esppai.prcid
						LEFT JOIN
							conjur.processoconjur prc ON prc.prcid = pv.pro_prcid
						LEFT JOIN
							conjur.tipoprocesso tpr ON tpr.tprid = prc.tprid
						LEFT JOIN
							conjur.estruturaprocesso esp ON prc.prcid = esp.prcid
						LEFT JOIN
							workflow.documento doc ON doc.docid = esp.docid
						LEFT JOIN
							workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE
							pv.prcid = ".$_SESSION['conjur_var']['prcid'];
                                }

                                $pvinculados = $db->carregar($sql);

                                if($pvinculados[0]) {
                                    foreach($pvinculados as $in) {
                                        echo "<tr>";
                                        echo "<td><center>".(($permissoes['gravar'])?$in['img']:"&nbsp")."</center></td>";
                                        echo "<td>".$in['numero']."</td>";
                                        echo "<td>".$in['tprdsc']."</td>";
                                        echo "<td>".$in['prcnomeinteressado']."</td>";
                                        echo "<td>".$in['prcdtentrada']."</td>";
                                        echo "<td>".$in['esddsc']."</td>";
                                        echo "</tr>";
                                    }
                                }
                                ?>
                            </table>
                        </td>
                    </tr>
                    <?
                    $estadosArquivar = Array(WF_ENCERRADO,
                        WF_AGUARDANDO_RESPOSTA_EXTERNA);
                    if($permissoes['gravar']&&!in_array($esdid,$estadosArquivar)&&(conjur_regraB( $cooid )||$db->testa_superuser()) ) {
                        ?>
                        <tr>
                            <td colspan="2"><input type="button" value="Anexar processo" onclick="abreJanelaProcessosVinculados();"></td>
                        </tr>
                    <? } ?>
                    <tr>
                        <td class="SubTituloEsquerda" colspan="2">Processo(s) Anexador(es)</td>
                    </tr>
                    <tr>
                        <td colspan="2">
                            <?php
                            $sql = "SELECT
							'<center><a style=\"cursor:pointer;\" onclick=\"verprocesso(' || prc.prcid || ');\">' || prcnumsidoc || '</a></center>',
							tprdsc, prcnomeinteressado,
							to_char(prcdtentrada, 'dd/mm/YYYY') as prcdtentrada, esd.esddsc
						FROM
							conjur.processosvinculados as pv
						LEFT JOIN
							conjur.processoconjur prc ON prc.prcid = pv.prcid
						LEFT JOIN
							conjur.tipoprocesso tpr ON tpr.tprid = prc.tprid
						LEFT JOIN
							conjur.estruturaprocesso esp ON prc.prcid = esp.prcid
						LEFT JOIN
							workflow.documento doc ON doc.docid = esp.docid
						LEFT JOIN
							workflow.estadodocumento esd ON esd.esdid = doc.esdid
						WHERE pv.pro_prcid = {$_SESSION['conjur_var']['prcid']}";

                            $cabecalho = array("Nº do Processo", "Tipo", "Nome do Interessado", "Data de Entrada", "Situação Conjur");

                            $db->monta_lista_simples($sql, $cabecalho, 50, 10, 'N', '100%', 'S');

                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td bgcolor="#CCCCCC" colspan="2" align="center">
                            &nbsp;
                            <? if($permissoes['gravar']) {?>
                                <input type="button" value="Salvar" style="cursor:pointer" id="botaosubmeter" onclick="editaFormCadastroProcesso();">
                            <? } ?>
                            <input type="button" value="Voltar" style="cursor:pointer" id="botaovoltar" onclick="window.location= 'conjur.php?modulo=inicio&acao=C';">
                        </td>
                    </tr>
                </table>
            </form>
            <div id="div_dialog_tramit_adv" style="display: none;">
                <form method="POST" id="formPrazoAdv_t" enctype="multipart/form-data" name="formPrazoAdv_t">
                    <div>
                        <h3>Definição do Prazo Advogado</h3>
                        <table>
                            <tr>
                                <td class="subtituloDireita" style="width:20%;">Prazo Advogado:</td>
                                <td>
                                    <?php
                                        echo campo_data('haddtprazoadv_t', 'S', 'S', '', 'S', '', '', null, 'formPrazoAdv_t');
                                    ?>
                                </td>
                            </tr>
                        </table>
                    </div>
                </form>
            </div>
        </td>
        <td>
            <div style="position: absolute; top: 400px; right: 33px;">
                <?php
                /* Barra de estado atual e ações e Historico */
                wf_desenhaBarraNavegacao( $docid, array( 'docid' => $docid, 'prcid' => $_SESSION['conjur_var']['prcid'], 'cooid' => $cooid, 'usar_acaoPossivel2' => true ) );
                ?>
            </div>
        </td>
    </tr>
</table>