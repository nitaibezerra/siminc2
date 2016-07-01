<?php
//$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
//if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
//	$_SESSION['obras2']['orgid'] = '';
//}
$_SESSION['obras2']['orgid'] = 3;
$orgid                       = $_SESSION['obras2']['orgid'];

switch ( $_REQUEST['req'] ){
	case 'abaAnaliseTerrenoObjeto':
		$_SESSION['obras2']['obrid'] = $_REQUEST['obrid'];
		$_SESSION['obras2']['preid'] = $_REQUEST['preid'];
		header('Location: ?modulo=principal/analiseTerrenoObjeto&acao=A');
		die();
//	case 'vistoriaEmpresaMI':
	case 'evolucaoMI':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid                       = $_SESSION['obras2']['obrid'];
		$obras                       = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/listaEvolucaoMi&acao=A');
		die();
	case 'supervisaoMI':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid                       = $_SESSION['obras2']['obrid'];
		$obras                       = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/listaSupervisaoMI&acao=A');
		die();
	case 'cadastrarOsMi':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid                       = $_SESSION['obras2']['obrid'];
		$obras                       = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/cadOsMi&acao=A&tomid='.$_POST['tomid']);
		die();
	case 'avaliarOsMi':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid                       = $_SESSION['obras2']['obrid'];
		$obras                       = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/popupAceiteOS&acao=A&tomid='.$_POST['tomid']);
		die();
	case 'abaFotos':
		$url = '/slideshow/slideshow/obras2_galeriaGaleriaFotos.php?tipo=abaGaleria&obrid=' . $_GET['obrid'];
		header('Location: ' . $url);
		die();
	case 'abaDocumento':
		$_SESSION['obras2']['obrid'] = $_POST['obrid'];
		$obrid                       = $_SESSION['obras2']['obrid'];
		$obras                       = new Obras( $obrid );
		$_SESSION['obras2']['empid'] = $obras->empid;
		header('Location: ?modulo=principal/cadObraDocumentos&acao=A');
		die();
	case 'pegaHtmlPagamento':
		if ($_REQUEST['preid']) {
			/* Substituido dia 23/12/2013 parnumseqob ->   pagnumeroob
			 * Analista: Daniel Areas Programador Eduardo Duniceu
			* */
			$sql = "SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento 
				FROM par.empenhoobra emo 
				INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
				INNER JOIN par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A'
				INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
				WHERE emo.preid = '".$_REQUEST['preid']."' AND pag.pagstatus='A'
				UNION ALL
				SELECT pro.proagencia, pro.probanco, pag.pagvalorparcela, pag.pagnumeroob, to_char(pag.pagdatapagamento,'dd/mm/YYYY') as pagdatapagamento
				FROM par.empenhoobrapar emo 
				INNER JOIN par.empenho emp ON emp.empid = emo.empid and empstatus = 'A' and eobstatus = 'A' 
				INNER JOIN par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso and pro.prostatus = 'A' 
				INNER JOIN par.pagamento pag ON pag.empid = emo.empid   
				WHERE emo.preid = '".$_REQUEST['preid']."' AND pag.pagstatus='A'";
			
			$pagamentoobra = $db->pegaLinha($sql);
			
			if($pagamentoobra) {
				echo "Pago<br>
					  Valor pagamento(R$): ".number_format($pagamentoobra['pagvalorparcela'],2,",",".")."<br> 
					  Nº da Ordem Bancária: ".$pagamentoobra['pagnumeroob']."<br> 
					  Data do pagamento: ".$pagamentoobra['pagdatapagamento']."<br>
					  Banco: ".$pagamentoobra['probanco'].", Agência: ".$pagamentoobra['proagencia']."
					  ";	
			} else {
				echo "Não pago";	
			}
			
		}	
		die();	
	case 'listaObrasUnidade':
		$obras 			 = new Obras();
		$filtro                  = $_POST;
		$filtro['not(obridpai)'] = true;
		$cabecalho		 = $obras->cabecalhoListaSql();
		$sql   			 = ($_POST ? $obras->listaSql( $filtro ) : array());
                $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");	
		die();
}

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

if(!empty($_REQUEST['ajax'])){
    switch ($_REQUEST['ajax']){
            case 'municipio':
                    header('content-type: text/html; charset=ISO-8859-1');
                    $municipio = new Municipio();
                    echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
                    die();		
    }
}

echo '<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />';

if($_GET['acao'] != 'AE'){
    //Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo "<br>";
}

switch ( $_GET['acao'] ){
	case 'O':
		$arAba = getArAba('listamiorgaoemissao');
		echo montarAbasArray($arAba, "?modulo=principal/listaObrasMI&acao=O&orgid=" . $orgid);
		monta_titulo( 'Lista de Obras MI - Emitir OS', 'Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.');
		break;
	case 'E':
		$arAba = getArAba('listamiorgaoaceite');
		echo montarAbasArray($arAba, "?modulo=principal/listaObrasMI&acao=E&orgid=" . $orgid);
		monta_titulo( 'Lista de Obras MI - Aceitar OS', 'Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.');
		break;
	case 'AE':
                if(isset($_GET['obrid']) || $_GET['obrid'] != ''){
                    $osMi = new OrdemServicoMI();
                    $imgs = $osMi->getBtnSinalizacaoOsMi($_GET['obrid']);
                    $img_exec = $imgs['execucao'];
                    $img_sond = $imgs['sondagem'];
                    $img_impl = $imgs['implantacao'];
                }else{
                    $img_exec = '';
                    $img_sond = '';
                    $img_impl = '';
                }
                
                //Popup Aceitar/Emitir OS
                echo ' 
                    <html>
                    <head>
                        <script type="text/javascript" src="../includes/JQuery/jquery-1.5.1.min.js"></script>
                        <script src="../library/chosen-1.0.0/chosen.jquery.js" type="text/javascript"></script>
                        <script src="../library/chosen-1.0.0/docsupport/prism.js" type="text/javascript"></script>
                        <link href="../library/chosen-1.0.0/chosen.css" rel="stylesheet"  media="screen" >
                        <script language="JavaScript" src="../includes/funcoes.js"></script>
                        <link href="/library/simec/css/css_reset.css" rel="stylesheet">
                        <link href="/library/simec/css/barra_brasil.css" rel="stylesheet">
                        <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
                        <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
                    </head>
                     ';
                //$db->cria_aba(ID_ABA_OS_MI, $url, $parametros); 
		monta_titulo( 'Lista de Obras MI - Popup Aceitar/Emitir OS', 'Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.');
                echo '<table align="center" bgcolor="#f5f5f5" border="1" class="Tabela" cellpadding="3" cellspacing="1">
                                <tr>
                                    <td class="SubTituloDireita" width="20%"><b>ID da Obra:</b></td>
                                    <td style="border:none">
                                          <input type="text" value="'.$_GET['obrid'].'" name="txt_obrid" id="txt_obrid">'.obrigatorio().' 
                                    </td>
                                <tr>
                                    <td class="SubTituloDireita" width="20%"><b>Tipo de OS:</b></td>
                                    <td style="border:none">
                                          <input type="radio" value="2" name="radio_tipo_os" id="radio_tipo_os_sond" > '.$img_sond.' OS de Sondagem &nbsp; 
                                          <input type="radio" value="3" name="radio_tipo_os" id="radio_tipo_os_prim" > '.$img_impl.' OS do Projeto de Implantação &nbsp;'.obrigatorio().' 
                                          <input type="radio" value="1" name="radio_tipo_os" id="radio_tipo_os_exec" > '.$img_exec.' OS de Execução &nbsp; 
                                    </td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita" width="20%"><b>Ação de OS:</b></td>
                                    <td style="border:none">
                                          <input type="radio" value="A" name="radio_acao_os" id="radio_acao_os_aceite" >Aceitar OS &nbsp;
                                          <input type="radio" value="E" name="radio_acao_os" id="radio_acao_os_emitir" >Emitir OS  &nbsp; '.obrigatorio().' 
                                    </td>
                                </tr>
                                <tr>
                                    <td class="SubTituloDireita" width="100%" colspan="2" style="text-align: center">
                                        <input type="button" value="Abrir" name="btn_popup_ae" id="btn_popup_ae">
                                    </td>                                    
                                </tr>
                        </table>' ;
		break;
	default:
		$arAba = getArAba('listamiorgao');
		echo montarAbasArray($arAba, "?modulo=principal/listaObrasMI&acao=A&orgid=" . $orgid);
		monta_titulo( 'Lista de Obras MI', 'Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.');
}

/**
 * EXTRAI OS DADOS DA PESQUISA PARA RECARREGAR O FORM
 */
extract( $_POST );
?>

<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
//<!--
var u='?modulo=principal/listaObrasMI&acao=A&req=pegaHtmlPagamento&preid=';

function visualizarOsObra(obrid){            
            var url = 'obras2.php?modulo=principal/listaObrasMI&acao=AE&obrid='+obrid;            
            window.open( url, "Aceite/Emição de OS", "width=750,height=600,scrollbars=yes,scrolling=no,resizebled=no");            
        };

$(document).ready(function(){

	$('.pesquisar').click(function(){
		$('#req').val('pesquisar');
		$('#formListaObra').submit();
	});
	$('.btnEexcel').click(function(){
		$('#req').val('excel');
		$('#formListaObra').submit();
	});

	$('.novaObra').click(function(){
		window.location.href = 'obras2.php?modulo=principal/cadObra&acao=A';
	});
        
        $('#aguarde').hide();
        
        
        $('#btn_popup_ae').click(function(){
            var obrid = $('#txt_obrid').val();
            var tomid = $("input:radio[name=radio_tipo_os]:checked" ).val();
            var acao  = $("input:radio[name=radio_acao_os]:checked" ).val();
            var url   = '';
            
            if(obrid == ''){
                alert('Informe o ID da Obra.');
                return false;                
            }
            
            if(tomid == undefined){
                alert('Escolha o Tipo de OS.');
                return false;
            }
            
            if(acao == undefined){
                alert('Escolha a Ação de OS.');
                return false;
            }
            
            switch(acao){
                case 'A':
                    url = 'obras2.php?modulo=principal/popupAceiteOS&acao=A&tomid='+tomid+'&obrid='+obrid;
                    break;
                case 'E':
                    url = 'obras2.php?modulo=principal/cadOsMi&acao=A&tomid='+tomid+'&obrid='+obrid;
                    break;
                default:
                    url = 'obras2.php?modulo=principal/cadOsMi&acao=A&tomid='+tomid+'&obrid='+obrid;
                    break;
            }            
            window.location.href = url;
            
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
		$('#abreBuscaAvancada').val( 't' );
	}else{
		$('#tr_busca_avancada').hide();
		$('#labelBuscaAvancada').show();
		$('#abreBuscaAvancada').val( 'f' );
	} 	
}

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
			  url  	     : url,
			  type       : 'post',
			  data       : {ajax: 'municipio', estuf: estuf},
			  dataType   : "html",
			  async	     : false,
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

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function supervisaoObrMi( obrid ){
	$('#req').val('evolucaoMI');
	$('#obrid').val(obrid);	
	$('#formListaObra').submit();
}

function cadastrarOS( obrid , tomid){
	$('#req').val('cadastrarOsMi');
	$('#obrid').val(obrid);
	$('#tomid').val(tomid);
	
	janela('' ,800, 650, 'cadastrarOsMi');	
	
	$('#formListaObra').attr('target', 'cadastrarOsMi');
	$('#formListaObra').submit();
        
        //Campos zerados para manter o alvo do formulário na mesma página, e não em outra aba como ocorria
        $('#req').val('');
	$('#obrid').val('');
	$('#tomid').val('');
	$('#formListaObra').attr('target', '');
}

function avaliarOS( obrid , tomid){
	$('#req').val('avaliarOsMi');
	$('#obrid').val(obrid);
	$('#tomid').val(tomid);
	
	janela('' ,800, 650, 'avaliarOsMi');	
	
	$('#formListaObra').attr('target', 'avaliarOsMi');
	$('#formListaObra').submit();
        //Campos zerados para manter o alvo do formulário na mesma página, e não em outra aba como ocorria
	$('#req').val('');
	$('#obrid').val('');
	$('#tomid').val('');
	$('#formListaObra').attr('target', '');
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function abreDocumentoObjeto( obrid ){
	$('#req').val('abaDocumento');
	$('#obrid').val(obrid);
	
	$('#formListaObra').submit();
}

function abreFotoObjeto( obrid ){
	janela('?modulo=principal/listaObrasMI&acao=A&req=abaFotos&obrid=' + obrid, 800, 650);
}

function abreAnaliseTerrenoObjeto( obrid, preid ){
	janela('?modulo=principal/listaObrasMI&acao=A&req=abaAnaliseTerrenoObjeto&obrid=' + obrid + '&preid=' + preid, 800, 650);
}


//-->
</script>


<?php

if($_GET['acao'] != 'AE'){

?>

<form method="post" name="formListaObra" id="formListaObra">
    <input type="hidden" name="req" id="req" value="">
    <input type="hidden" name="obrid" id="obrid" value="">
    <input type="hidden" name="empid" id="empid" value="">
    <input type="hidden" name="tomid" id="tomid" value="">
    <input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
            <td>
                <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
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
                        <td class="SubTituloDireita" width="20%">Empresa</td>
                        <td>
                            <?php
                                $empresami = new EmpresaMi();
                                $sql = $empresami->listaCombo(false);
//                                $sql = $empresami->listaComboComEstuf();
                                $db->monta_combo("emiid", $sql, 'S', "Selecione...", "", '', '', '', 'N', 'emiid');
                            ?>
                        </td>
                    </tr>					
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Situação:</td>
                        <td>
                            <?php
                            $sql = "SELECT 
                                            esdid as codigo, 
                                            esddsc as descricao 
                                    FROM 
                                            workflow.estadodocumento 
                                    WHERE 
                                            tpdid='" . TPDID_OBJETO . "' AND 
                                            esdstatus='A' 
                                    ORDER BY 
                                            esdordem";

                            $db->monta_combo("esdid", $sql, "S", "Todos", "", "", "", "200", "N", "esdid");
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
                        <td>
                        <?php
                            $tipoObra = new TipoObra();
                            $db->monta_combo("tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid");
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
                        <td>
                        <?php
                            $classificacaoObra = new ClassificacaoObra();
                            $db->monta_combo("cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid");
                        ?>
                        </td>
                    </tr>
                    <?php
                    if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL){
                        ?>
                            <tr>
                                <td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
                                <td>
                                <?php
                                    $tipologiaObra = new TipologiaObra();
                                    $param = array("orgid" => $_SESSION['obras2']['orgid'], "IS(mi)" => true);
                                    $db->monta_combo("tpoid", $tipologiaObra->listaCombo($param), "S", "Todas", "", "", "", 200, "N", "tpoid");
                                ?>
                                </td>
                            </tr>
                        <?php
                    }
                    ?>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Programa:</td>
                        <td>
                        <?php
                            $programa = new ProgramaFonte();
                            $db->monta_combo("prfid", $programa->listaCombo(array("orgid" => $_SESSION['obras2']['orgid'])), "S", "Todos", "", "", "", 200, "N", "prfid");
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td style="width:20%" class="SubTituloDireita">Fonte</td>	
                        <td>
                        <?php
                            $tipoFormaRepasseRecursos = new TipoFormaRepasseRecursos();
                            $db->monta_combo('frpid', $tipoFormaRepasseRecursos->listaCombo(), $habilita, "Selecione...", 'abreCamposRecursos', '', '', '200', 'N', 'frpid');
                        ?>	
                        </td>
                    </tr>

                        <?php if ($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL){ ?>
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
                            <td>
                            <?php
                                $modalidade = new ModalidadeEnsino();
                                $db->monta_combo("moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid");
                            ?>
                            </td>
                        </tr>
                    <?php } ?>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Unidade:</td>
                        <td>
                        <?php
                            $entidade = new Entidade();
                            $db->monta_combo("entid", $entidade->listaComboObras2(), "S", "Todos", "", "", "", 200, "N", "entid");
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Última atualização:</td>
                        <td>
                        <?php
                            $ultatualizacao = $_POST["ultatualizacao"];

                            $arSel = array( array("codigo" => 1, "descricao" => "Em até 45 dias"),
                                            array("codigo" => 2, "descricao" => "Entre 45 e 60 dias"),
                                            array("codigo" => 3, "descricao" => "Mais de 60 dias")
                            );

                            $db->monta_combo("ultatualizacao", $arSel, "S", "Todos", "", "", "", "200", "N", "ultatualizacao");
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">UF:</td>
                        <td>
                        <?php
                            $uf = new Estado();
                            $db->monta_combo("estuf", $uf->listaCombo(), 'S', 'Selecione...', 'carregarMunicipio', '', '', 200, 'N', 'estuf');
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Município:</td>
                        <td id="td_municipio">
                        <?php
                            if ($estuf) {
                                $municipio = new Municipio();
                                $dado = $municipio->listaCombo(array('estuf' => $estuf));
                                $habMun = 'S';
                            } else {
                                $dado = array();
                                $habMun = 'N';
                            }
                            $habMun = ($disable == 'N' ? $disable : $habMun);
                            echo $db->monta_combo("muncod", $dado, $habMun, 'Selecione...', '', '', '', 200, 'N', 'muncod');
                        ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Esfera:</td>
                        <td>
                        <?php
                            $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                                         Array('codigo' => 'M', 'descricao' => 'Municipal'));
                            $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
                        ?>
                        </td>
                    </tr>
                        <?php if ($_GET['acao'] == 'O'){ ?>										
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Possui OS:</td>
                            <td>
                                <input type="radio" name="osexiste" id="" value="S" <?= ( $_POST["osexiste"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                                <input type="radio" name="osexiste" id="" value="N" <?= ( $_POST["osexiste"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                                <input type="radio" name="osexiste" id="" value=""  <?= ( $_POST["osexiste"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                            </td>
                        </tr>
                        <?php } ?>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui foto:</td>
                        <td>
                            <input type="radio" name="totalfoto" id="" value="S" <?= ( $_POST["totalfoto"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="totalfoto" id="" value="N" <?= ( $_POST["totalfoto"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="totalfoto" id="" value=""  <?= ( $_POST["totalfoto"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui vistoria:</td>
                        <td>
                            <input type="radio" name="possui_vistoria" id="" value="S" <?= ( $_POST["possui_vistoria"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="possui_vistoria" id="" value="N" <?= ( $_POST["possui_vistoria"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="possui_vistoria" id="" value=""  <?= ( $_POST["possui_vistoria"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui restrição:</td>
                        <td>
                            <input type="radio" name="res_estado" id="" value="S" <?= ( $_POST["res_estado"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="res_estado" id="" value="N" <?= ( $_POST["res_estado"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="res_estado" id="" value=""  <?= ( $_POST["res_estado"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Restrição superada:</td>
                        <td>
                            <input type="radio" name="res_superada" id="" value="S" <?= ( $_POST["res_superada"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="res_superada" id="" value="N" <?= ( $_POST["res_superada"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="res_superada" id="" value=""  <?= ( $_POST["res_superada"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <?php if ($_GET['acao'] != 'O'){ ?>
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Possui Aditivo:</td>
                            <td>
                                <input type="radio" name="ocraditivado" id="" value="S" <?= ( $_POST["ocraditivado"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                                <input type="radio" name="ocraditivado" id="" value="N" <?= ( $_POST["ocraditivado"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                                <input type="radio" name="ocraditivado" id="" value=""  <?= ( $_POST["ocraditivado"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                            </td>
                        </tr>
                    <?php } ?>					
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui Supervisão:</td>
                        <td>
                            <input type="radio" name="responsavel" id="" value="S" <?= ( $_POST["responsavel"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="responsavel" id="" value="N" <?= ( $_POST["responsavel"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="responsavel" id="" value=""  <?= ( $_POST["responsavel"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui Evolução MI:</td>
                        <td>
                            <input type="radio" name="evomi" id="" value="S" <?= ( $_POST["evomi"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="evomi" id="" value="N" <?= ( $_POST["evomi"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="evomi" id="" value=""  <?= ( $_POST["evomi"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Possui Contrato Anexado:</td>
                        <td>
                            <input type="radio" name="arqidcontrato" id="" value="S" <?= ( $_POST["arqidcontrato"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                            <input type="radio" name="arqidcontrato" id="" value="N" <?= ( $_POST["arqidcontrato"] == "N" ? "checked='checked'" : "" ) ?>/> Não
                            <input type="radio" name="arqidcontrato" id="" value=""  <?= ( $_POST["arqidcontrato"] == "" ? "checked='checked'" : "" ) ?> /> Todas
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Tipo Vistoria (Realizado por):</td>
                        <td>
                        <?php
                            $realizacaosupervisao = new RealizacaoSupervisao();
                            $db->monta_combo("rsuid", $realizacaosupervisao->listaCombo(), "S", "Todos", "", "", "", 200, "N", "rsuid");
                        ?>
                        </td>
                    </tr>
                    <?php if ($_GET['acao'] != 'O'){ ?>
                        <tr>
                            <td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
                            <td>
                                De:&nbsp;
                                <?php
                                    echo campo_texto('obrvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                ?>
                                Até:&nbsp;
                                <?php
                                    echo campo_texto('obrvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                ?>
                            </td>
                        </tr>
                    <?php } ?>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">Nível de Preenchimento:</td>
                        <td>
                            <?php
                                $nivelpreenchimento = $_POST["nivelpreenchimento"];
                                $arrNivel = array( array("codigo" => 1, "descricao" => "Amarelo ( Obras atualizadas entre 45 e 60 dias )"),
                                                   array("codigo" => 2, "descricao" => "Azul ( Obras concluídas )"),
                                                   array("codigo" => 3, "descricao" => "Verde ( Obras atualizadas há menos de 45 dias atrás )"),
                                                   array("codigo" => 4, "descricao" => "Vermelho ( Obras atualizadas há mais de 60 dias )")
                                );
                                $db->monta_combo("nivelpreenchimento", $arrNivel, "S", "Todos", "", "", "", "", "N", "nivelpreenchimento");
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita" style="width: 190px;">% Executado da Obra:</td>
                        <td>
                            <table>
                                <tr>
                                    <th>Mínimo</th>
                                    <th>Máximo</th>
                                </tr>
                                <tr>
                                <?php
                                    for ($i = 0; $i <= 100; $i++) {
                                        $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                                    }

                                    $percentualinicial = $_POST['percentualinicial'];
                                    $percentualfinal   = $_POST['percentualfinal'];
                                    $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                                    print '<td>';
                                    $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                                    print '</td><td>';
                                    $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                                    print '</td>';
                                ?>
                                </tr>
                            </table>
                        </td>
                    </tr>
                </table>			
            </td>
        </tr>
        <tr>
            <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input type="button" name="btnEexcel" class="btnEexcel" value="Gerar Excel"/>
            </td>
        </tr>
    </table>
</form>

<?php
}

if ( ($_POST || !possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )) && ($_GET['acao'] != 'AE') ){

		$obras                   = new Obras();
		$filtro                  = $_POST;
		$filtro['not(obridpai)'] = true;
		$filtro['IS(mi)']        = true;
		
		switch ( $_GET['acao'] ){
			case 'O':
                                // ESDID_OBJ_EXECUCAO temporario, enquanto existir obras MI em execução sem cronograma.
//				$filtro['esdid']    = array(ESDID_OBJ_EXECUCAO, ESDID_OBJ_AGUARDANDO_EMISSAO_OS,ESDID_OBJ_AGUARDANDO_EMISSAO_OS_COM_PENDENCIA, ESDID_OBJ_AGUARDANDO_ACEITE_OS);
				$filtro['tipoAcao'] = 'aguardandoEmissao';
				$cabecalho          = array("Ação", "OS", "R", "I", "ID", "ID <br>Pré-Obra",
                                                            "Obra", "Empresa", "Unidade Implantadora", "Município",  "Bairro",
                                                            "Situação da<br>Obra", "Última Atualização", "Última Vistoria", 
                                                            "%<br> Executado", "Tipologia");
				break;
			case 'E':
//				$filtro['possuios'] 	= ESDID_OBJ_AGUARDANDO_ACEITE_OS;
				$filtro['tipoAcao'] = 'aguardandoAceite';
				$cabecalho = array("Ação", "OS", "R", "I", "ID", "ID <br>Pré-Obra", "Nº Processo", "Ano Processo", "Termo/Convênio",
                                                    "Ano do<br> Termo/Convênio", "Obra", "Empresa", "Unidade Implantadora", "Município",  "Bairro",
                                                   "Data de Início<br>da Execução", "Data de Término<br>da Execução", "Situação da<br>Obra", 
                                                   "Última Atualização", "Última Vistoria", "%<br> Executado", "Tipologia",
                                                   "Valor Contrato", "Soma das Evoluções MI");
				break;
			default:
				$cabecalho = array("Ação", "OS", "R", "I", "ID", "ID <br>Pré-Obra","Nº Processo", "Ano Processo", "Termo/Convênio",
                                                    "Ano do<br> Termo/Convênio", "Obra", "Empresa", "Unidade Implantadora", "Município",  "Bairro",
                                                   "Data de Início<br>da Execução", "Data de Término<br>da Execução", "Situação da<br>Obra", 
                                                   "Última Atualização", "Última Vistoria", "%<br> Executado", "Tipologia",
                                                   "Valor Contrato", "Soma das Evoluções MI");
				
		}

                
		if( $filtro['req'] == 'excel' ){
                    $sql = $obras->listaMiSql($filtro);
                    $cabecalho = array( "ID", "ID <br>Pré-Obra", "Nº Processo", "Ano Processo", "Termo/Convênio",
                                        "Ano do<br> Termo/Convênio", "Obra", "Empresa", "Unidade Implantadora", "Município", "Bairro",
                                        "Data de Início<br>da Execução", "Data de Término<br>da Execução", "Situação da<br>Obra",
                                        "Última Atualização", "Última Vistoria", "%<br> Executado", "Tipologia",
                                        "Valor Contrato", "Soma das Evoluções MI");
//            ver(simec_htmlentities($sql), d);
                    $obras->listaObrasExcelMi( $sql, $cabecalho );
		} else {
                    if($_POST['req'] == 'pesquisar'){
                        $sql = $obras->listaMiSql($filtro);
                        $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
                    }
		}

}else{
    
?>

<?php
    if($_GET['acao'] != 'AE'){
?>
	<table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
            <tr>
                <td style="text-align: justify;">
                    Para listar as obras, escolha os argumentos de pesquisa desejados e clique em pesquisar. Se nada for escolhido serão apresentados todos os registros que você pode acessar.		
                </td>
            </tr>
	</table>		
<?php	
    }else{
?>
        <table width="95%" cellspacing="0" cellpadding="2" border="0" align="center" class="listagem">
            <tr>
                <td style="text-align: justify;">
                    Informe o ID da Obra, o Tipo de O.S. a Ação desejada e clique em Abrir para ver os dados da Ordem de Serviços.
                </td>
            </tr>
        </table>
<?php
    }
}
?>