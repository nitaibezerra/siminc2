<?php 
if( $_REQUEST['tela'] == 'repactuacao' ){ 
    
    header( 'Content-type: text/html; charset=iso-8859-1' );
    
    include_once "config.inc";
    include_once APPRAIZ . "includes/funcoes.inc";
    include_once APPRAIZ . "includes/classes_simec.inc";
    include_once APPRAIZ . "includes/classes/Modelo.class.inc";
    include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
    
    $requisicao   = $_REQUEST['requisicao'];
    $vgcid        = $_REQUEST['vgcid'];
    $ctrid        = $_REQUEST['ctrid'];
    $rpcid        = $_REQUEST['rpcid'];
    $rpcvalor     = $_REQUEST['rpcvalor'];
    $rpcdtinicio  = $_REQUEST['rpcdtinicio'];
    $rpcdtfim     = $_REQUEST['rpcdtfim'];
    $vgcdtinicio  = $_REQUEST['vgcdtinicio'];
    $vgcdtfim     = $_REQUEST['vgcdtfim'];
    $arqid        = null;
    $rpcdocumento = $_REQUEST['rpcdocumento'];

    // Ação do botão salvar
    if( $requisicao == 'salvar' ){
    
        if( !empty($rpcid) ){
    
            try {
                 
                $updateVigencia = "UPDATE fabrica.repactuacaocontrato
                                    SET rpcdtinicio = '".formata_data_sql($rpcdtinicio)."'
                                    , rpcdtfim         = '".formata_data_sql($rpcdtfim)."'
                                    , rpcvalor         = ". str_replace(',', '.', $rpcvalor) ."
                                    , rpcdocumento     = '".$rpcdocumento."'
                                    , vgcid            = ". $vgcid ."
                                    , ctrid            = ". $ctrid ."
                                    , tprid            = 1
                                    , rpcstatus        = 'A'
                                    WHERE rpcid = " . $rpcid;
                //die($updateVigencia);
                $msg = "Repactuação atualizada com sucesso.";
                $db->executar( $updateVigencia ) ;
                $db->commit( ) ;
                 
            } catch (Exception $e) {
    
                $db->rollback();
                $msg = $e->getMessage();
            }
    
        }else{
    
            try {
    
                $campos = array('rpcdtinicio'   => "'".formata_data_sql($rpcdtinicio)."'"
                        ,'rpcdtfim'     => "'".formata_data_sql($rpcdtfim)."'"
                        ,'rpcvalor'     => str_replace(',', '.', str_replace('.', '', $rpcvalor))
                        ,'rpcdocumento' => "'".$rpcdocumento."'"
                        ,'vgcid'        => $vgcid
                        ,'ctrid'        => $ctrid
                        ,'tprid'        => 1
                        ,'rpcstatus'    => "'A'");
    
                if($_FILES['arquivo'] && $_FILES['arquivo']['error'] == 0) {
    
                    $msg = "Repactuação cadastrada com sucesso.";
    
                    $file = new FilesSimec("repactuacaocontrato", $campos ,"fabrica");
                    $file->setUpload( (($rpcdocumento)?$rpcdocumento:NULL), $key = "arquivo" );
    
                }else{
                    throw new Exception( "Não foi possível cadastrar a repactuação." );
                }
    
                $db->commit();
    
            } catch (Exception $e) {
    
                $db->rollback();
                $msg = $e->getMessage();
            }
        }
    }
    
    // Carrega dados da repactuação
    if ( !empty($rpcid) && empty($_REQUEST['rpcvalor']) ){
        
        $sqlRep = "select a.ctrid, a.vgcid, a.rpcvalor, a.rpcdtinicio, a.rpcdtfim, a.rpcdocumento, a.arqid, b.vgcdtinicio, b.vgcdtfim ";
        $sqlRep.= " from fabrica.repactuacaocontrato a ";
        $sqlRep.= " inner join fabrica.vigenciacontrato b on a.vgcid = b.vgcid and b.vgcstatus = 'A' ";
        $sqlRep.= " where a.rpcstatus = 'A' and a.rpcid=".$rpcid;
        
        $resultado  = $db->pegaLinha( $sqlRep );
        
        if( !empty($resultado) ){
        
            $ctrid        = $resultado['ctrid'];
            $vgcid        = $resultado['vgcid'];
            $rpcvalor     = $resultado['rpcvalor'];
            $rpcdtinicio  = formata_data( $resultado['rpcdtinicio'] );
            $rpcdtfim     = formata_data( $resultado['rpcdtfim'] );
            $vgcdtinicio  = formata_data( $resultado['vgcdtinicio'] );
            $vgcdtfim     = formata_data( $resultado['vgcdtfim'] );
            $rpcdocumento = $resultado['rpcdocumento'];
            $arqid        = $resultado['arqid'];
        }

    // Carrega dados da vigência
    }elseif( !empty($vgcid) ){

        $sqlRep = "select ctrid, vgcdtinicio, vgcdtfim ";
        $sqlRep.= " from fabrica.vigenciacontrato ";
        $sqlRep.= " where vgcstatus = 'A' and vgcid = ". $vgcid;
        
        $resultado  = $db->pegaLinha( $sqlRep );
        
        if( !empty($resultado) ){
        
            $ctrid        = $resultado['ctrid'];
            $vgcdtinicio  = formata_data( $resultado['vgcdtinicio'] );
            $vgcdtfim     = formata_data( $resultado['vgcdtfim'] );
            
            // Pega a data fim da vigencia para um novo registro de repactuacao
            $rpcdtfim     = formata_data( $resultado['vgcdtfim'] );

        }else{
            throw new Exception( "Não foi possível cadastrar a repactuação." );
        }
    }

    ?>
    <html>
        <head>
            <script language="JavaScript" src="../includes/funcoes.js"></script>
            <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
            <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
            
            <script type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
            <script type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
            <script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
            <link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
                
            <script type="text/javascript">

        	function mostrarAquivo(arqid)
        	{
        		if(arqid){
        			window.open('fabrica.php?modulo=principal/cadContrato&acao=A&download=S&arqid='+arqid,'Download','scrollbars=yes,height=250,width=520,status=no,toolbar=no,menubar=no,location=no');
        		}else{
        			alert('Não é possível carregar janela de repactuação!');
        		}
        	}

        	function salvarRepactuacao()
        	{
        		var erro = 0;

        		$("[class~=obrigatorio]").each(function() { 
        			if(!this.value){
        				erro = 1;
        				alert('Favor preencher todos os campos obrigatórios!');
        				this.focus();
        				return false;
        			}
        		});

        		if(erro == 0){
        			if( !checarDatas() ){
        				return false;
        			}
        		}else{
        			return false;
        		}
            }

        	function checarDatas(){
        		
        		if(document.getElementById('rpcdtinicio').value && document.getElementById('rpcdtfim').value) {

        		    // Datas da repactuacao
        			var data_1 = document.getElementById('rpcdtinicio').value;
        			var data_2 = document.getElementById('rpcdtfim').value;

        			var comparaRep_1 = parseInt(data_1.split("/")[2].toString() + data_1.split("/")[1].toString() + data_1.split("/")[0].toString());
        			var comparaRep_2 = parseInt(data_2.split("/")[2].toString() + data_2.split("/")[1].toString() + data_2.split("/")[0].toString());
        			
        			// Data das vigencia
        			var dataV_1 = '<?php echo($vgcdtinicio);?>';
        		    var dataV_2 = '<?php echo($vgcdtfim);?>';

        			var comparaVig_1 = parseInt(dataV_1.split("/")[2].toString() + dataV_1.split("/")[1].toString() + dataV_1.split("/")[0].toString());
        			var comparaVig_2 = parseInt(dataV_2.split("/")[2].toString() + dataV_2.split("/")[1].toString() + dataV_2.split("/")[0].toString());

        			if (comparaRep_1 > comparaRep_2) {

        				alert('Data de início não pode ser maior que a data de término');

        			}else if(comparaRep_1 < comparaVig_1) {

        				alert('Data de início não pode ser menor que a data de início da Vigencia');

        			}else if(comparaRep_2 > comparaVig_2) {

        				alert('Data de término não pode ser maior que a data de término da Vigencia');
        				
        			}else{
        				var rpcid = $('[name=rpcid]').val();
        				var vgcid = $('[name=vgcid]').val();
        				
        				$.ajax({
        					   type: "get",
        					   url:  'fabrica.php?modulo=principal/cadContrato&acao=A',
        					   data: "periodorepactuacaovalido=ok&rpcdtinicio=" + data_1 +"&rpcdtfim=" + data_2 + "&rpcid=" + rpcid + "&vgcid=" + vgcid,
        					   dataType: 'json',
        					   success: function(msg){
        					     if( msg.status == 'true' ){
        					    	$('#formulario_').submit();
        					     }else{
        					     	alert('Período de repactuacao inválido.');
        							return false;
        					     }
        					   }
        					 });
        			}
        		}	

        		return false;
        	}

            $(document).ready(function() {

            	//window.onbeforeunload = function (){
            	//	window.opener.location = window.opener.location;
            	//	window.close(); 
            	//}
            });
            </script>
        </head>
        
        <body>
        <?php 
        $titulo_modulo = "Repactuação";
        monta_titulo($titulo_modulo, '');
        ?>
        <form method="post" name="formulario_" id="formulario_" enctype="multipart/form-data">

        <input type="hidden" name="requisicao" value="salvar">
        <input type="hidden" name="tela"       value="repactuacao">
        <input type="hidden" name="rpcid"      value="<?php echo $rpcid; ?>">
        <input type="hidden" name="vgcid"      value="<?php echo $vgcid; ?>">
        <input type="hidden" name="ctrid"      value="<?php echo $ctrid; ?>">
            
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
        <tr>
        	<td width="95%" valign="top">
            	<table class="listagem" width="100%">
            	<tr>
            		<td class="SubTituloDireita" width="40%">Tipo:</td>
            		<td>Repactuação de Valores</td>
            	</tr>
            	<tr>
            		<td class="SubTituloDireita">Valor Ponto de Função Repactuação:</td>
            		<td><?=campo_texto("rpcvalor", "S", "S", "Valor Ponto de Função Repactuação", "20", "120", "###.###.###,##",'','','','','','',$rpcvalor); ?></td>
            	</tr>
            	<tr>
            		<td class="SubTituloDireita">Data de Início:</td>
            		<td><?=campo_data2("rpcdtinicio","S","S","Data de Início","##/##/####"); ?></td>
            	</tr>
            	<tr>
            		<td class="SubTituloDireita">Data de Término:</td>
            		<td><?=campo_data2("rpcdtfim","S","S","Data de Término","##/##/####"); ?></td>
            	</tr>
            	<tr>
            		<td class="SubTituloDireita">Nº Documento:</td>
            		<td><?=campo_texto("rpcdocumento", "S", "S", "Nº Documento", "30", "50", "", ""); ?></td>
            	</tr>
                
                <?php if( empty($arqid) ) { ?>
            	<tr>
            		<td class="SubTituloDireita">Arquivo:</td>
            		<td><input type="file" name="arquivo" id="arquivo">
            		<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
            		</td>
            	</tr>
                <?php }else{ ?>
            	<tr>
            		<td class="SubTituloDireita">Arquivo:</td>
            		<td> <a href="javascript:void(0);" onclick="mostrarAquivo( <?php echo($arqid);?> )">Abrir Documento</a> </td>
            	</tr>
                <?php } ?>
            	<tr>
            	    <td class="SubTituloDireita"></td>
            		<td><input type="button" onclick="salvarRepactuacao();" name="btn_gravar" value="Salvar"></td>
            	</tr>
            	</table>
        	</td>
        </tr>
        </table>
        </form>
        </body>
    </html>
    <?php if($msg): ?>
    <script>
    	alert('<?php echo $msg ?>');
    	window.opener.location = window.opener.location;
    	window.close();
    </script>
    <?php endif; ?>
<?php
}else{
    $clsFuncoes = new funcoesProjetoFabrica();
    
    if($_REQUEST['requisicao']){
    	$entid = $_REQUEST['requisicao']();
    	Header("Location: fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&entid=$entid&tipo=".$_REQUEST['tipo']."&msg=1");
    }
    
    require_once APPRAIZ . "includes/classes/entidades.class.inc";
    
    $entid = $_REQUEST['entid'];
    
    if($_REQUEST['tipo'] == TIPO_EMPRESA_FABRICA){
    	$titulo = "Dados da Empresa";
    }
    if($_REQUEST['tipo'] == TIPO_GESTOR_CONTRATO){
    	$titulo = "Dados do Gestor / Co-Gestor";
    }
    
    monta_titulo( $titulo, '&nbsp;' );
    
    $entidade = new Entidades();
    
    if($entid){
    	$entidade->carregarPorEntid($entid);
    }
    echo $entidade->formEntidade("fabrica.php?modulo=principal/popup/entidadeContrato&acao=A&requisicao=".$clsFuncoes->salvarRegistroEntidade."&tipo=".$_REQUEST['tipo'],
    							 array("funid" => (int)$_REQUEST['tipo'])
    							 );
    							 
    //array perfis							 
    $pfls = arrayPerfil();
    	
    ?>
    <script language="JavaScript" src="../includes/funcoes.js"></script>
    <script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
    <link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
    <script>

    	jQuery.noConflict();
    	jQuery('#btngravar').click(function() {
    
    		jQuery('#entnumcpfcnpj').attr('disabled', false);
    		jQuery('#njuid').attr('disabled', false);
    		
    		if(jQuery('#entid').val()){
    			<?php if($_REQUEST['tipo'] == TIPO_EMPRESA_FABRICA): ?>
    				jQuery('[name=entnome_empresa]', window.opener.document).val(jQuery('#entnome').val());
    				jQuery('[name=entidcontratado]', window.opener.document).val(jQuery('#entid').val());
    				jQuery('#img_add_empresa', window.opener.document).attr("src","../imagens/alterar.gif");
    			<?php endif; ?>
    			<?php if($_REQUEST['tipo'] == TIPO_GESTOR_CONTRATO): ?>
    				<?php if($_REQUEST['funcao'] == "gestor"): ?>
    					jQuery('[name=entnome_gestor_contrato]', window.opener.document).val(jQuery('#entnome').val());
    					jQuery('[name="entidgestor"]', window.opener.document).val(jQuery('#entid').val());
    					jQuery('#img_add_gestor', window.opener.document).attr("src","../imagens/alterar.gif");
    				<?php endif; ?>
    				<?php if($_REQUEST['funcao'] == "cogestor"): ?>
    					jQuery('[name=entnome_co_gestor_contrato]', window.opener.document).val(jQuery('#entnome').val());
    					jQuery('[name="entidcogestor"]', window.opener.document).val(jQuery('#entid').val());
    					jQuery('#img_add_co_gestor', window.opener.document).attr("src","../imagens/alterar.gif");
    				<?php endif; ?>
    			<?php endif; ?>
    			
    		}
    	});
    	<?php if($_REQUEST['tipo'] == TIPO_EMPRESA_FABRICA): ?>
    		jQuery('#tr_entcodent,#tr_entnuninsest,#tr_entunicod,#tr_entungcod,#tr_tpctgid,#tr_tpcid,#tr_tplid,#tr_funcoescadastradas,#tr_tpsid').remove();
    	<?php endif; ?>
    	<?php if($_REQUEST['tipo'] == TIPO_GESTOR_CONTRATO): ?>
    		jQuery('#tr_funid').hide();
    	<?php endif; ?>
    
    	<?php if( !in_array(PERFIL_SUPER_USUARIO,$pfls) && !in_array(PERFIL_FISCAL_CONTRATO,$pfls) ): ?>
    		if(jQuery('#entnumcpfcnpj').val()){
    			jQuery('#entnumcpfcnpj').attr('disabled', true);
    			jQuery('#njuid').attr('disabled', true);
    		}
    	<?php endif;?>
    	
    	
    </script>
    <?php if($_REQUEST['msg']): ?>
    	<script>
    		alert('Operação Realizada com Sucesso!');
    		window.close();
    	</script>
    <?php endif; 
}?>