<?php

$orgid 		             = $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['obrid'] = (!isset($_SESSION['obras2']['obrid']) && isset($_GET['obrid'])) ? $_GET['obrid'] : $_SESSION['obras2']['obrid'];
$obridSession                = $_SESSION['obras2']['obrid'];

// empreendimento || obra || orgao
verificaSessao( 'obra' );

if (isset($_GET['fase']) == false){
	$_SESSION['obras2']['idFaseLicitacao'] = array();
}
$licid = $db->pegaUm(" SELECT licid FROM obras2.obralicitacao WHERE oblstatus = 'A' AND obrid = " . $obridSession);

if(!$licid){
    // Cria uma nova licitação
    $obraLicitacao = new ObraLicitacao();
    $licitacao = new Licitacao();

    $licitacao->orgid = 3;
    $licitacao->molid = null;
    $licitacao->licstatus = 'A';
    $licid = $licitacao->salvar();

    $obraLicitacao->licid = $licid;
    $obraLicitacao->obrid = $obridSession;
    $obraLicitacao->oblstatus = 'A';
    $obraLicitacao->salvar();

//    $obraLicitacao->commit();
//    $licitacao->commit();
}


$_SESSION['obras2']['licid'] = $licid;

$licitacao = new Licitacao( $licid );
if ( $licitacao->licid ){
	extract( $licitacao->getDados() );
}
switch ( $_REQUEST['requisicao'] ){
	case 'salvar':
		$licid = $licitacao->popularDadosObjeto( $_POST )
				  		   ->salvar();

                 foreach ($_POST['obrid'] as $obrid) {
                     $obra = new Obras($obrid);
                     registraAtividade(
                                    'Alteração da Licitação Obra: ('.$obra->obrid.') ' . $obra->obrnome, 
                                    $_POST['rgadsccompleta'], 
                                    $arqidcusto);
                 } 
		$db->commit();				  		   
		$_SESSION['obras2']['licid'] = $licid;	

		die("<script>
				alert('Operação realizada com sucesso!');window.opener.location.reload();
			 </script>");
				  
		break;
            
	case 'excluirfase':
		$faseLicitacao = new FaseLicitacao( $_POST['flcid'] );
                $sql = "SELECT
                        distinct obrid
                FROM
                        obras2.obralicitacao ol
                JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND
                fl.flcstatus = 'A'
                WHERE fl.flcid = {$_POST['flcid']} AND ol.oblstatus = 'A'";
                
                $obras = $db->carregar($sql);                
                $obras = (!empty($obras)) ? $obras : array();
                
                foreach ($obras as $obra) {
                     $obra = new Obras($obra['obrid']);
                     registraAtividade(
                                    'Exclusão da Fase da Licitação Obra: ('.$obra->obrid.') ' . $obra->obrnome, 
                                    $_POST['rgadsccompleta'], 
                                    $arqidcusto);
                 } 
                
		$faseLicitacao->flcstatus = 'I';
		$faseLicitacao->salvar();
		$db->commit();				  		   
		exit;
	case "download":
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$arqid = $_REQUEST['arqid'];
		$file = new FilesSimec();
	    $arquivo = $file->getDownloadArquivo($arqid);
		die();
}

function registraAtividade($descricaocurta, $descricao, $arqid = null, $rgaid = null){
    global $db;
    
    $regAtividade = new RegistroAtividade();
    $arDado = array();

    $arDado['arqid'] = $arqid;
    $arDado['obrid'] = $_SESSION['obras2']['obrid'];
    $arDado['usucpf'] = $_SESSION['usucpf'];
    $arDado['rgadscsimplificada'] = $descricaocurta;
    $arDado['rgadsccompleta'] = $descricao;

    $arCamposNulo = array();

    if ( empty($arDado['arqid']) )
        $arCamposNulo[] = 'arqid'; 	

    $rga = $regAtividade->popularDadosObjeto( $arDado )
                 ->salvar(true, true, $arCamposNulo);
    $db->commit();
    
    return $rga;
}


monta_titulo( 'Editar Licitação', '<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif"> Indica Campo Obrigatório.');
?>

<html>
<head>
	<title>Editar Licitação</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
        <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
	<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
        <script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
</head>

<body topmargin="0" leftmargin="0">    
    <form method="post" id="formulario" name="formulario">
    <input type="hidden" name="requisicao" id="requisicao" value="salvar"/>
    <input type="hidden" name="licid" id="licid" value="<?php echo $licid ?>"/>
    <table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
            <tr>
                    <td width="265" class="subtitulodireita">Órgão:</td>
                    <td>
                            <?php 
                            $orgao = new Orgao();
                            echo $orgao->pegaDescricao( $orgid );
                            ?>
                            <input type="hidden" name="orgid" id="orgid" value="<?php echo $orgid; ?>">
                    </td>
            </tr>
            <tr>
                    <td width="265" class="subtitulodireita">Modalidade de Licitação:</td>
                    <td>
                            <?php 
                                    $modalidade = new ModalidadeLicitacao();
                                    $db->monta_combo('molid', $modalidade->listaCombo(), 'S', "Selecione...", 'abreNumLicUasg', '', '', '', 'S', 'molid'); 
                            ?>
                    </td>
            </tr>
            <tr>
                    <td>Fases de Licitação</td>
            </tr>
            <tr>
                    <td colspan="2">
                            <table id="faseslicitacao" width="70%" align="center" border="0" cellspacing="2" cellpadding="2" class="listagem">
                                    <thead>
                                            <tr bgcolor="#e9e9e9">
                                                    <td width="10%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ação</strong></td>
                                                    <td width="70%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Descrição</strong></td>
                                                    <td width="20%" valign="middle" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Data</strong></td>						
                                            </tr>
                                    </thead>
                                    <?php 
                                            if($licid || $_GET['fase']){

                                                    $faseLic = new FaseLicitacao();
                                                    $arDados = $faseLic->listaPorLicitacao( $licid, ($_GET['fase'] ? $_SESSION['obras2']['idFaseLicitacao'] : array()) );

                                                    foreach ($arDados as $k => $dados){
    //						while (($dados = pg_fetch_assoc($sql))){
                                                            $flcid 				= $dados['flcid'];
                                                            $tflid 				= $dados['tflid'];
                                                            $tfldesc 			= $dados['tfldesc'];
                                                            $flcrecintermotivo 	= $dados['flcrecintermotivo'];
                                                            $flcordservnum 		= $dados['flcordservnum'];
                                                            $flcpubleditaldtprev = formata_data($dados['flcpubleditaldtprev']);
                                                            $flcdtrecintermotivo = formata_data($dados['flcdtrecintermotivo']);
                                                            $flcordservdt 		= formata_data($dados['flcordservdt']);
                                                            $flchomlicdtprev 	= formata_data($dados['flchomlicdtprev']);
                                                            $flcaberpropdtprev 	= formata_data($dados['flcaberpropdtprev']);
                                                            $tflordem 			= $dados['tflordem'];
                                                            $flcmeiopublichomol = $dados['flcmeiopublichomol'];
                                                            $flcobshomol 		= $dados['flcobshomol'];
                                                            $arqid		 		= $dados['arqid'];
                                                            $arqnome	 		= $dados['arqnome'];
                                                            $arqextensao 		= $dados['arqextensao'];

                                                            if($tflid ==2){
                                                                    $_SESSION["obras2"]["flcpubleditaldtprev"] = formata_data_sql($flcpubleditaldtprev);
                                                                    $flcdata = $flcpubleditaldtprev;
                                                            }
                                                            if($tflid ==5){
                                                                    $flcdata = $flcdtrecintermotivo;
                                                            }
                                                            if($tflid ==6){
                                                                    $flcdata = $flcordservdt;
                                                            }
                                                            if($tflid ==9){
                                                                    $flcdata = $flchomlicdtprev;
                                                                    $_SESSION["obras2"]["flchomlicdtprev"] = $flchomlicdtprev; 
                                                            }
                                                            if($tflid ==7){
                                                                    $flcdata = $flcaberpropdtprev;
                                                            }


                                                            if($somenteLeitura == "N"){
                                                                    $botoes = "<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\">";
                                                            }else{
                                                                    $botoes = " <img src='/imagens/alterar.gif' style='cursor: pointer' border=0 title='Editar' onclick=\"atualizaFase(".$flcid.",'update');\"/>
                                                                                            <img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\"  border=0 title=\"Excluir\" onClick=\"RemoveLinha(window.document.getElementById('tr_".$flcid."').rowIndex,'".$flcid."');\">
                                                                                            ";
                                                                    if ( $arqid ){
                                                                            $botoes .= "&nbsp;<img src='/imagens/anexo.gif' style='cursor: pointer' border=0 title='Baixar arquivo' onclick=\"DownloadArquivo(" . $arqid . ");\"/>";
                                                                    }else{
                                                                            $botoes .= "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;";
                                                                    }
                                                            }

                                                            echo "
                                                                    <tr id=\"tr_" . $flcid . "\">
                                                                            <td bgcolor=\"#F7F7F7\" align=\"center\">" . $botoes . "</td>
                                                                            <td><input type='hidden' name='flcid[]' id='flcid_".$flcid."'  value='".$flcid."'/>" . $tfldesc . "</td>
                                                                            <td>
                                                                            <input type='hidden' name='flcpubleditaldtprev[]' id='flcpubleditaldtprev_".$flcid."' value='".$flcpubleditaldtprev."'/>
                                                                            <input type='hidden' name='flcdtrecintermotivo[]' id='flcdtrecintermotivo_".$flcid."' value='".$flcdtrecintermotivo."'/>
                                                                            <input type='hidden' name='flcordservdt[]' id='flcordservdt_".$flcid."' value='".$flcordservdt."'/>
                                                                            <input type='hidden' name='flchomlicdtprev[]' id='flchomlicdtprev_".$flcid."' value='".$flchomlicdtprev."'/>
                                                                            <input type='hidden' name='flcaberpropdtprev[]' id='flcaberpropdtprev_".$flcid."' value='".$flcaberpropdtprev."'/>									
                                                                            <input type='hidden' name='flcdata[]' id='flcdata_".$flcid."' value='".$flcdata."'/>
                                                                            " . $flcdata . "
                                                                            <input type='hidden' name='flcrecintermotivo[]' id='flcrecintermotivo_".$flcid."' value='".$flcrecintermotivo."'/>
                                                                            <input type='hidden' name='flcobshomol[]' id='flcobshomol_".$flcid."' value='".$flcobshomol."'/>
                                                                            <input type='hidden' name='flcmeiopublichomol[]' id='flcmeiopublichomol_".$flcid."' value='".$flcmeiopublichomol."'/>
                                                                            <input type='hidden' name='flcordservnum[]' id='flcordservnum_".$flcid."' value='".$flcordservnum."'/>
                                                                            <input type='hidden' name='tflordem[]' id='flordem_".$flcid."' value='".$tflordem."'/>		
                                                                            <input type='hidden' name='tflid[]' id='tflid_".$flcid."' value='".$tflid."'/>
                                                                            </td>
                                                                    </tr>
                                                            ";
                                                    }
                                            }

                                    ?>
                            </table>
                            <div id='conteinerAcao'>			
                            </div>
                    </td>
            </tr>
            <tr>
                    <td>
                            <a href="#" onclick="adicionarFases('licitacao');"><img src="/imagens/gif_inclui.gif" style="cursor:pointer;" border="0" title="Inserir Fases"> Inserir Fases de Licitação </a>
                    </td>
            </tr>
            <tr id="tr_numlic" style="display: none;">
                    <td class="subtitulodireita">Número da Licitação:</td>
                    <td>
                            <?php 
                                    $licnumero = $licitacao->licnumero;
                                    echo campo_texto( 'licnumero', 'N', 'S', '', 12, 10, '##########', '', 'left', '', 0, 'id="licnumero"' ); 
                            ?>
                    </td>
            </tr>
    <?php 
            if ( empty($obridSession) ):	
    ?>
            <tr>
                    <td class="SubTituloDireita" valign="top">Obra(s):</td>
                    <td valign="top">
                    <?
                    $obra 		 = new Obras();
                    $filtro 	 = array('naoLicitada'   => ($licid ? $licid : true), 
                                                             'orgid'	     => $orgid,
                                                             'not(obridpai)' => true);

                    $sql 		 = $obra->listaComboPopupSql( $filtro );
                    $obrid		 = ($obridSession && empty($licid) ? $obra->carregaComboPopupPorObra( $obridSession ) : $obra->carregaComboPopupPorLicitacao( $licid ));

                    $filtro = array(
                                                    array("codigo" 		=> "obrnome",
                                                              "descricao" 	=> "<b>Nome:</b>",
                                                              "string" 		=> "1")
                                                    );
                    combo_popup('obrid', $sql, 'Selecione a(s) Obra(s)', "400x400", 0, array(), "", "S", false, false, 4, 330, null, null, false, $filtro);
                    ?>
                    </td>
            </tr>
    <?php
            endif;
    ?>	
            
            <tr>
                    <td width="265" class="subtitulodireita">Justificativa: </td>
                    <td>
                                    <?php 
                                    echo campo_textarea( 'rgadsccompleta', 'N', 'S', '', 100, 20, 5000 );
                                    ?>
                    </td>
            </tr>
            <tr bgcolor="#DEDEDE">
                    <td colspan="2">
                    <?php
                    if ( $obridSession ):
                    ?>
                            <input type="hidden" name="obrid[]" id="obrid" value="<?php echo $obridSession; ?>"/>
                            <input type="button" name="botao" value="Salvar e Voltar" onclick="salvaLicitacao();"/>
                            <input type="button" name="botao" value="Voltar" onclick="window.location='?modulo=principal/exibeLicitacao&acao=A';"/>
                    <?php
                    else:
                    ?>
                            <input type="button" name="botao" value="Salvar" onclick="salvaLicitacao();"/>
                            <input type="button" name="botao" value="Fechar" onclick="window.close()"/>
                    <?php 
                    endif;
                    ?>	
                    </td>
            </tr>
    </table>
    </form>
</body>
<script type="text/javascript">
DownloadArquivo = function(arqid){
	window.location = '?modulo=principal/cadLicitacao&acao=E&requisicao=download&arqid='+arqid;
}

function adicionarFases( objeto ){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=A&objeto='+objeto+'&rca=true','blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function abreNumLicUasg( id ){
	
	var tr_uasg    = document.getElementById( 'tr_uasg' );
	var tr_numlic  = document.getElementById( 'tr_numlic' );

	if( id == 1 || id == 2 || id == 3 || id == 4 ){
		tr_numlic.style.display = (document.selection ? 'block' : 'table-row');
	}else{
		tr_numlic.style.display = 'none';
	}
}

abreNumLicUasg(<?php echo $molid ?>);

function salvaLicitacao(){

	var obData = new Data();

	var tflid = document.getElementsByName("tflid[]");
	var molid = document.getElementById( "molid" ).value;
	
	var flcdata = document.getElementsByName('flcdata[]');
	
	if( (tflid.length == 0) /*&& (superuser == 1)*/){
		alert( "Favor inserir ao menos uma das Fases de Licitação!" );
		return false;
	}else{
		var flag = false;
	
		for( i = 0; i < tflid.length; i++ ){
			
			if( Number(tflid[i].value) == 2 || Number(tflid[i].value) == 9 ){
				flag = true;
				break;
			}
		}
	}

	if( !flag /*&& (superuser == 1)*/){
		alert("Favor inserir a fase Publicação do Edital ou Homologação da Licitação!");
		return false;
	}

	if ( molid == '' /*&& (superuser == 1)*/){
		alert( "O campo Modalidade de Licitação deve ser preenchido!" );
		return false;
	}else{
		if ( document.getElementById('obrid').tagName == 'SELECT' ){
			selectAllOptions(document.getElementById('obrid'));
		}	
		document.getElementById( "formulario" ).submit();
	}
}

function atualizaFase(flcid){
	return windowOpen( '?modulo=principal/vincFaseLicitacao&acao=A&flcid='+flcid+'&rca=true','blank','height=450,width=650,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function RemoveLinha(index,id){
        if(!jQuery('#rgadsccompleta').val()){
            alert('Preencha a justificativa.');
            return false;
        }
	if( confirm('Tem certeza que deseja excluir este registro?') ){
		table = window.document.getElementById("faseslicitacao");
//		var conteiner = window.document.getElementById("conteinerAcao");
//		conteiner.innerHTML += "<input type='hidden' name='acaoFases[]' id='acaoFases"+id+"' value='"+id+"'/>";
		table.deleteRow(index);
		
                
		jQuery.ajax({
				  url  		 : '?modulo=principal/popupEditarLicitacao&acao=E',
				  type 		 : 'post',
				  data 		 : {requisicao : 'excluirfase', 
                                                    flcid : id,
                                                    rgadsccompleta: 'teste'},
				  dataType   : "html",
				  async		 : false,
				  beforeSend : function (){
				  	divCarregando();
				  },
				  error 	 : function (){
				  	divCarregado();
				  },
				  success	 : function ( data ){
				  	alert('Fase excluída com sucesso.');
                                        console.info(data);
				  	divCarregado();
				  }
				});		
	}
}	

</script>
<?php
         if(possui_perfil(PFLCOD_CALL_CENTER)){
             $blockEdicao = TRUE;
         }
        if($blockEdicao){
            echo '<script type="text/javascript">';
            echo " setTimeout(bloqueiaForm('formulario'), 500);
                   function bloqueiaForm(idForm){
                      jQuery('#'+idForm).find('input, textarea, button, select').attr('disabled','disabled');
                      jQuery('#'+idForm).find('a, span').attr('onclick','alert(\"Você não pode editar os dados da Obra Vinculada.\")');
                      jQuery('#voltar').attr('disabled', false);
                   }
                 ";
            echo '</script>';
        }
?>