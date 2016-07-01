<?php
include APPRAIZ ."includes/workflow.php";

//echo'<pre>';var_dump($_REQUEST);

$sqlCategoria = 'SELECT ctp.ctpid as codigo, ctp.ctpdsc as descricao
                 FROM fabrica.categoriapausa ctp
                 INNER JOIN fabrica.tipodocumento_categoriapausa tdcp
                 ON ctp.ctpid = tdcp.ctpid
                 WHERE tdcp.tpdid = %d';

//Fluxo - Solicitação Serviço
if(isset( $_REQUEST['scsid'] )) 
{
	$sqlCategoria = sprintf($sqlCategoria,  WORKFLOW_SOLICITACAO_SERVICO);

//Fluxo - Ordem de serviço    
}elseif(isset($_REQUEST['odsid'])) 
{
    $sqlCategoria = sprintf($sqlCategoria,  WORKFLOW_ORDEM_SERVICO);
}

//var_dump( $_REQUEST );
//var_dump( $sqlCategoria );

monta_titulo($titulo, "");

if(isset($_POST['acaoSalvar']))
{
	$ctpdsc = $db->pegaUm( 'SELECT ctpdsc FROM fabrica.categoriapausa WHERE ctpid = ' . $_REQUEST['ctpid'] );
	$dadosAlterarEstado = $ctpdsc . " - ". $_REQUEST['obsdsc'];

	//echo '<pre>';
	//var_dump(  $_REQUEST );exit;
	
    //Fluxo salvar - Solicitação Serviço
	if(!empty( $_REQUEST['scsid'] )){
		
		$sqlDocId = "select 
						docid 
					from 
						fabrica.solicitacaoservico
					where 
						scsid = ". $_REQUEST['scsid'];

		$retornoSql = $db->pegaLinha( $sqlDocId );
		
		if( !empty( $retornoSql['docid'] ) )
		{
			//$dadosVerificacao  = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";
			$dadosVerificacao	= array();
			$esdidorigem 		= wf_pegarEstadoAtual(  $retornoSql['docid'] );
			$rsPegarAcao 		= wf_pegarAcao( $esdidorigem['esdid'], WF_ESTADO_SS_PAUSA);

			wf_alterarEstado( $retornoSql['docid'], $rsPegarAcao['aedid'], $dadosAlterarEstado, $dadosVerificacao);

			// Alteração para quem vem da tela Painel Operacional
			if( !empty( $_REQUEST['alteracao'] ) ){ ?>
				<script type="text/javascript">
	
					alert( 'Estado alterado com sucesso!' );
					window.opener.location.reload();
					</script>
			<?php
			}else{
			?>
			<script type="text/javascript">

				window.opener.wf_atualizarTela( 'Estado alterado com sucesso!', self );
				
			</script>
	    	<?php
			} 
		}
	
    //Fluxo salvar - Ordem de serviço    
    } elseif(!empty($_REQUEST['odsid'])){
    	
    	$sqlDocId = "select
			    		case when tosid = " . TIPO_OS_GERAL . "
			    		then docid
			    		else docidpf
			    		end AS doc_id
			    		, tosid
			    	from 
			    		fabrica.ordemservico
    				where 
    					odsid = ". $_REQUEST['odsid'];

		$retornoSql = $db->pegaLinha( $sqlDocId );
		
		//echo '<pre>';
		//var_dump(  $sqlDocId );exit;
		
		if( !empty( $retornoSql['doc_id'] )  && !empty( $retornoSql['tosid'] ) )
		{
			//$dadosVerificacao  = (array) "a:1:{s:6:\"unicod\";s:0:\"\";}";
			$dadosVerificacao	= array();
			$esdidPausa			= (TIPO_OS_GERAL == $retornoSql['tosid'] ? WF_ESTADO_OS_PAUSA : WF_ESTADO_CPF_PAUSA) ;
			$esdidorigem 		= wf_pegarEstadoAtual(  $retornoSql['doc_id'] );

			//Tratamento para recarregamento de página
			if( $esdidorigem['esdid'] != $esdidPausa)
			{
				$rsPegarAcao = wf_pegarAcao( $esdidorigem['esdid'], $esdidPausa);
				wf_alterarEstado( $retornoSql['doc_id'], $rsPegarAcao['aedid'], $dadosAlterarEstado, array( 'odsid' => $_REQUEST['odsid'] ));
			}
			?>
			<script type="text/javascript">

				alert( 'Estado alterado com sucesso!' );
				window.opener.location.reload();
				//window.close();
				//alert( 'Estado alterado com sucesso!' );
				//window.opener.location.reload();
				//window.close();
				/*
				window.opener.location.href = window.opener.location.href;
				//window.opener.location = window.location;
				self.close();
				window.opener.location.reload();
				*/
				//window.opener.wf_atualizarTela( 'Estado alterado com sucesso!', self );
				//window.opener.wf_atualizarTela( 'Estado alterado com sucesso!', self );
			</script>
	    	<?php 
		}
    }

    

}
?>
<script type="text/javascript" src="./js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>

<script type="text/javascript">

$(document).ready(function(){
	
    $("#btnSalvar").click(function(){

        var resultado = false;
        var texto 	  = '';

        if( $('#ctpid').val() == ''){

			alert('Informe uma categoria para a pausa.');
			$('#ctpid').focus();

        }else{
        	if( $('#obsdsc').val() == ''){
            	alert('Informe uma justificativa para a pausa.');
    			$('#obsdsc').focus();            	
        	}else{
        		resultado = true;
            }            
        }

        //Enviar formulario
		if( resultado ){
			$("#formulario").submit();
		}
	});	        
});

</script>

<form id="formulario" name="formulario" action="" method="post" >
    <table id="tblCurso" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
        <tr>
            <td class="SubTituloDireita" style="width:200px;"> Categoria: </td>
            <td>
                <?php 
                $db->monta_combo('ctpid', $sqlCategoria, 'S', 'Selecione', '', '', '', '', 'S', 'ctpid', '', $_REQUEST['ctpid']);
                ?>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" style="width:200px;"> Justificativa: </td>
            <td><?php echo campo_textarea('obsdsc', 'S', 'S', $label, 70, 8, 6000) ?> </td>
        </tr>
        <tr>
            <th colspan="2" style="text-align: center;">
                <input type="hidden" value="<?php echo $_GET['odsid']; ?>" name="odsid" id="odsid" />
                <input type="hidden" value="<?php echo $_GET['scsid']; ?>" name="scsid" id="scsid" />
                <input type="hidden" value="<?php echo $_GET['alteracao']; ?>" name="alteracao" id="alteracao" />
                <input type="hidden" value="ok" name="acaoSalvar" id="acaoSalvar" />
                <input type="button" value="Salvar" name="btnSalvar" id="btnSalvar" />
                <input type="button" value="Voltar" name="btnVoltar" id="btnVoltar" onclick="window.close();" />
            </th>
        </tr>
    </table>
</form>