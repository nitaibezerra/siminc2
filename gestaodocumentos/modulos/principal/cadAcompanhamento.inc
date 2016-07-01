<?php
    require_once APPRAIZ . "gestaodocumentos/classes/GestaoDocumentos.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/Solicitante.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/Acompanhamento.class.inc";
    require_once APPRAIZ . "gestaodocumentos/classes/Atividade.class.inc";
    require_once APPRAIZ . "includes/classes/dateTime.inc";
    require_once APPRAIZ . "includes/classes/AbaAjax.class.inc";

    if($_REQUEST['requisicao']=='cadastrar_atividade'){
    	$obAtividade = new Atividade();
    	$atv = $obAtividade->salvarAtividade($_POST);
    	// ver($atv,d);
    	extract($_POST);
    	if($atv)
    		listaAtendimento($tarid);
    	die();
   	}

   	if($_REQUEST['requisicao']=='alterar_atividade'){
    	 $obAtividade = new Atividade();
    	 $atv = $obAtividade->alterarAtividade($_POST);
    	 extract($_POST);
   		listaAtendimento($tarid);
    	die();
   	}

    if($_REQUEST['requisicao']=='alterar_atividade_2'){
        $obAtividade = new Atividade();
        $atv = $obAtividade->alterarAtividade2($_POST);
        extract($_POST);
        listaAtendimento($tarid);
        die();
    }
   	
   	if($_REQUEST['requisicao']=='excluir_atividade'){
    	 $obAtividade = new Atividade();
    	 $atv = $obAtividade->excluirAtividade($_POST);
    	 extract($_POST);
   		 listaAtendimento($tarid);
   		 die();
   	}   

    if($_REQUEST['requisicao']=='visualizar_atividade'){
    	 $obAtividade = new Atividade();
    	 $atv = $obAtividade->visualizarAtividade($_POST);
		 echo simec_json_encode($atv);
		 die();
    }     	
   	
    
    if($_GET['tarid']){
        # Verifica se existe tarefa
        boExisteTarefa( $_GET['tarid'], true );
    }

    #AÇÃO GRAVAR
    if($_POST['acaoForm']){
        extract($_POST);

        $obTarefa = new GestaoDocumentos();

        $arCampos = array('tarid','unaidsetororigem');
        $obTarefa->popularObjeto($arCampos);

        if($tarprioridade){
            $obTarefa->tarprioridade = $tarprioridade;
        }

        if($unaidsetorresponsavel){
            $obTarefa->unaidsetorresponsavel = $unaidsetorresponsavel;
        } else {
            $obTarefa->unaidsetorresponsavel = $unaidsetorresponsavelAnterior;
        }

        if($sitid){
            $obTarefa->sitid = $sitid;
        } else {
            $obTarefa->sitid = $sitidAnterior;
        }

        if($tardepexterna){
            $obTarefa->tardepexterna = $tardepexterna;
        } else {
            $obTarefa->tardepexterna = $tardepexternaAnterior;
        }

        $obData = new Data();
        if($tardataprazoatendimento){
            $obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimento,"YYYY-mm-dd");
        } else {
            $obTarefa->tardataprazoatendimento = $obData->formataData($tardataprazoatendimentoAnterior,"YYYY-mm-dd");		
        }

        # Verifica responsável
        if($usucpfresponsavel){
            $obTarefa->usucpfresponsavel = $usucpfresponsavel;	
        } else {
            if($boGravaNullCpfRespon > 0){
                $obTarefa->usucpfresponsavel = $usucpfresponsavelAnterior;	
            } else {
                $obTarefa->usucpfresponsavel = null;
            }
        }

        //echo $boGravaNullCpfRespon;exit;

        //ver($obTarefa->usucpfresponsavel,d);
        $arCamposNulo = array('usucpfresponsavel');

        $obTarefa->salvar(true, true, $arCamposNulo);
        if($acodsc){
         $obTarefa->salvarAcompanhamento($_POST);
        }
        
        $obTarefa->commit();

    /*	$_REQUEST['acao'] = 'A';
        $db->sucesso("principal/cadAcompanhamento","&tarid=".$obTarefa->tarid);
        unset($obTarefa);		
        die;*/
    }

    if($_GET['tarid'] && $_REQUEST['acao'] == "A"){
        $tarid = $_GET['tarid'];
    } elseif($_SESSION['tarid'] && $_REQUEST['acao'] == "A"){
        $tarid = $_SESSION['tarid'];
    } elseif($_SESSION['dados_tarefa']['tarid']){
        $tarid = $_SESSION['dados_tarefa']['tarid'];
    }

    if($tarid){
        # Verifica se existe tarefa
        boExisteTarefa( $tarid, true );
    }

    #OBJETO TAREFA
    $obTarefa = new GestaoDocumentos($tarid);
    if($obTarefa->tarid){
        $_SESSION['tarid'] = $obTarefa->tarid;
        $_SESSION['_tartarefa'] = $_GET['tartarefa'];
    } else {
        die("<script>alert('Tarefa não encontrada. Refaça o procedimento.');window.location='gestaodocumentos.php?modulo=principal/listaTarefas&acao=A';</script>");
    }

    if($_GET['taridExcluir']){
        $obTarefa = new GestaoDocumentos();
        $obTarefa->excluir($_GET['taridExcluir']);
        $obTarefa->commit();
        unset($_GET['taridExcluir']);
        unset($obTarefa);
        $db->sucesso("principal/cadAcompanhamento");
        die;
    }

    if($_REQUEST['requisicao'] != '') {
        $_REQUEST['requisicao']($_REQUEST);
        die();
    } 

    #Chamada de programa
    include  APPRAIZ."includes/cabecalho.inc";
    echo'<br>';
    $db->cria_aba( $abacod_tela, $url, '' );
    $titulo_modulo = "Acompanhamento da Demanda";
    monta_titulo( $titulo_modulo, '' );
    
    require_once 'cabecalhoIncludesListaTarefa.inc';

    if($_SESSION['gt_doc']['filtroArvore']){
        extract($_SESSION['gt_doc']['filtroArvore']);
    }
    
?>
<script type="text/javascript" src="./js/gestaodocumentos.js"></script>

<form method="post" name="formulario" id="formulario" action="/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A">
    <input type="hidden" name="cadAcompanhamento" id="cadAcompanhamento" value="1" />
    <input type="hidden" name="boGravaNullCpfRespon" id="boGravaNullCpfRespon" value="1" />
    <input type="hidden" name="acaoForm" id="acaoForm" value="1" />
    <input type="hidden" name="tarid" id="tarid" value="<? echo $obTarefa->tarid; ?>" />
    <input type="hidden" name="tardataprazoatendimentoTarefa" id="tardataprazoatendimentoTarefa" value="<? echo $_SESSION['dados_tarefa']['tardataprazoatendimento']; ?>" />
    <input type="hidden" name="boAtividade" id="boAtividade" value="<? echo $obTarefa->boAtividade(); ?>" />
    
    <br>

    <?php
        #CRIA ABA
        $obAbaAjax = new AbaAjax(null, false, true, true);

        # DEFINE ABA PADRÃO
        $padraoAtendimento = $padraoRestricao = false;

        if(!$_GET['boPadraoRetricao']){
            $padraoAtendimento = true;
        } else {
            $padraoRestricao = true;
        }

        $arAba = array(
            0 => array("descricao" => "Atendimento",
                       "padrao" => $padraoAtendimento,
                       "elemento" => array( "tarid" ),
                       "url" => "ajax.php",
                       "parametro" => "tipo=abaAtendimento"),
            1 => array("descricao" => "Restricao",
                       "padrao" => $padraoRestricao,
                       "elemento" => array( "tarid" ),
                       "url" => "ajax.php",
                       "parametro" => "tipo=abaRestricao")
        );

        $obAbaAjax->criaAba($arAba, 'divAcompanhamento');
    ?>
</form>

<script type="text/javascript">
	jQuery.noConflict();

    function enviaForm(){
	 	var nomeform 		= 'formulario';
		var submeterForm 	= true;
		var campos 			= new Array();
		var tiposDeCampos 	= new Array();
		
		campos[0] 			= "unaidsetorresponsavel";
		campos[1] 			= "usucpfresponsavel";
		campos[2] 			= "tarprioridade";
		campos[3] 			= "tardatainicio";
		campos[4] 			= "tardataprazoatendimento";
		campos[5] 			= "sitid";
		//campos[6] 			= "acodsc";
		
		tiposDeCampos[0] 	= "select";
		tiposDeCampos[1] 	= "select";
		tiposDeCampos[2] 	= "radio";
		tiposDeCampos[3] 	= "data";
		tiposDeCampos[4] 	= "data";
		tiposDeCampos[5] 	= "select";
		//tiposDeCampos[6] 	= "textarea";
		
		if(document.formulario.usucpfresponsavel.disabled == true){
			if(document.formulario.usucpfresponsavel.value == document.formulario.usucpfresponsavelAnterior.value){
				if(document.formulario.usucpfresponsavel.value == ''){
					document.formulario.boGravaNullCpfRespon.value = 1;
				} else {
					document.formulario.boGravaNullCpfRespon.value = '';
				}
			} else {
				document.formulario.usucpfresponsavel.value = '';
			}
			
		}
        
		validaForm(nomeform, campos, tiposDeCampos, submeterForm )
	}
	
	function excluirTarefaAtividade(tarid){
		if(confirm('Deseja excluir este registro')){
			window.location.href = "/gestaodocumentos/gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A&taridExcluir="+tarid;
			return true;
		} else {
			return false;
		}
	}
	
	function carregaCabecalhoAtendimento(tarid, celula){
		$('tarid').value = tarid;
		$('aguarde_').show();  
		
		//SE ESTIVER NA TELA DO DADOS DO ATENDIMENTO CARREGA BLOCO DE ATENDIMENTO
		if($('boRestricao') == null){
			var data = 'tipo=abaAtendimento&tarid='+tarid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,   
				onComplete: function(r)
				{
					$('divAtendimento').update(r.responseText);
					var tabela = $('tabela_tarefa');
					for(i=1; i<tabela.rows.length; i++){
						if(i % 2){
							tabela.rows[i].style.backgroundColor = '#fafafa';
						} else {
							tabela.rows[i].style.backgroundColor = '#f0f0f0';
						}
					}
					celula.style.background = '#ffffcc';
				}
			});
	
			var data = 'tipo=carregaListaAtendimento&tarid='+tarid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,  
				asynchronous: false, 
				onComplete: function(r)
				{
					$('divListaAtendimento').update(r.responseText);
				}
			});
		
		} else { // SENAO CARREGA DADOS E LISTA DA RESTRICAO
			var data = 'tipo=abaRestricao&tarid='+tarid+'&boNaoMostraTitulo=false';
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,   
				onComplete: function(r)
				{
					$('divRestricao').update(r.responseText);
					var tabela = $('tabela_tarefa');
					for(i=1; i<tabela.rows.length; i++){
						if(i % 2){
							tabela.rows[i].style.backgroundColor = '#fafafa';
						} else {
							tabela.rows[i].style.backgroundColor = '#f0f0f0';
						}
					}
					celula.style.background = '#ffffcc';
				}
			});
		}
		
		$('aguarde_').hide();
		return false;
	}
	
	function gravarRestricao(resid, _tartarefa){
		var residTemp  = ''; 
		var ressolucao = 'f';
		if(resid){
			residTemp = '_'+resid;
			var ressolucaoCheck = document.getElementsByName('ressolucao'+residTemp);
			for (var j = 0; j < ressolucaoCheck.length; j++) {
				if(ressolucaoCheck[j].checked == true){
					ressolucao = ressolucaoCheck[j].value;
				}
			}
		}

		var resdescricao = $('resdescricao'+residTemp).value;
        if(resdescricao == ''){
            alert('Restrição não pode ficar vazio.');
            return false;
        }
		var resmedida    = $('resmedida'+residTemp).value;
		var tarid        = $('tarid').value
		var data 		 = 'tipo=salvarRestricao&resid='+resid+'&resdescricao='+resdescricao+'&resmedida='+resmedida+'&tarid='+tarid+'&ressolucao='+ressolucao;
		var boGravar = false;
		new Ajax.Request('ajax.php',
		{  
			method: 'post',   
			parameters: data,
			asynchronous: false, 
			onLoading: $('aguarde_').show(),  
			onComplete: function(r)
			{
				$('resdescricao').value = "";
				$('resmedida').value = "";
				$('divListaRestricao').update(r.responseText);
				boGravar = true;
			}
		});
		
		if(boGravar){
			//montaArvoreAberta(_tartarefa, '', 1);
			$('aguarde_').hide();
			alert('Dados gravados com sucesso.');
		}
	}
	
	function excluirRestricao(resid, tarid){
		if(confirm('Deseja excluir este registro')){
			var data = 'tipo=excluirRestricao&tarid='+tarid+'&resid='+resid;
			new Ajax.Request('ajax.php',
			{  
				method: 'post',   
				parameters: data,
				asynchronous: false,
				onLoading: $('aguarde_').show(),
				onComplete: function(r)
				{
					$('divListaRestricao').update(r.responseText);
				}
			});
			$('aguarde_').hide();
			return true;
		} else {
			return false;
		}
	}
	
	function alteraCampoDescricao(resid){
		var idDiv1 = "divDesc1_"+resid;
		var idDiv2 = "divDesc2_"+resid;
		var bntAlterar = "bntAltera_"+resid;
		
		if($(idDiv1).style.display == 'none'){
			$(idDiv1).style.display = "";
			$(idDiv2).style.display = "none";
			$(bntAlterar).disabled = true;
		}
	}
	
	function gravarAtividade(){

        // default: cadastrar_atividade
        var valor = '';
        if( jQuery('[name=atvdetalhe]').val() != '' ){
            var elementoNovo = jQuery('[name=atvdetalhe]').val();
            valor += 'requisicao=cadastrar_atividade';
            valor += '&atvdetalhe='+elementoNovo;
            valor += '&tarid='+jQuery("#tarid").val();
            // da lista
            jQuery.each( jQuery('.excluir') ,function( index, value ){
                valor += '&atvids_'+index+'='+jQuery(value).attr('id');
            });
            jQuery.each( jQuery('#divListaAtendimento #atvdetalhe'), function( index, value ){
                valor += '&atvdetalhe_'+index+'='+jQuery(value).val();
            });
        // quando não tem item novo
        }else{
            valor.atvids = [];
            valor.atvdetalhe = [];
            valor += 'requisicao=alterar_atividade_2';
            valor += '&tarid='+jQuery("#tarid").val();
            // da lista
            jQuery.each( jQuery('.excluir') ,function( index, value ){
                valor += '&atvids_'+index+'='+jQuery(value).attr('id');
            });
            jQuery.each( jQuery('#divListaAtendimento #atvdetalhe'), function( index, value ){
                valor += '&atvdetalhe_'+index+'='+jQuery(value).val();
            });
        }

        jQuery.ajax({
			url: 'gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A',
			data: valor,
            type: 'POST',
			async: false,
			success: function(data) {
				if( trim(data) ) {
					jQuery("#atvdetalhe").val('');
					alert('Dados gravados com sucesso!');
					jQuery("#divListaAtendimento").html(data);
				} else {
					alert('Não foi possível gravar os dados!');
				}
	    	}
		});
	}

	function excluirAtividade(atvid,tarid){
		if(confirm("Deseja realmente excluir a Atividade?")){
			jQuery.ajax({
				url: 'gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A',
				type: 'POST',
				data: { requisicao: 'excluir_atividade', atvid: atvid, tarid: tarid},
				async: false,
				success: function(data) {
					alert('Atividade excluída com sucesso!');
					jQuery("#divListaAtendimento").html(data);
				}
			});
		}
	}	

	function visualizarAtividade(atvid,tarid){
		jQuery.ajax({
			url: 'gestaodocumentos.php?modulo=principal/cadAcompanhamento&acao=A',
			type: 'POST',
			data: { requisicao: 'visualizar_atividade', atvid: atvid, tarid: tarid},
			async: false,
			success: function(data) {
			    var data = jQuery.parseJSON(data);
			    jQuery('#atvid').val(data.atvid);
			    jQuery('#formatendimento #requisicao').val('alterar_atividade');
			    jQuery('#atvdetalhe').val(data.atvdetalhe);
			}
		});
	}	
</script>