<?php

include  APPRAIZ."includes/cabecalho.inc";

$titulo_modulo = "Vistas Disponibilizadas";
$modulo = "";
monta_titulo( $titulo_modulo, $modulo );

// dependências --------------------------------------------------------------------------------------------------
include APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . "cproc/classes/ModeloCproc.class.inc";
include_once APPRAIZ . "cproc/classes/Processo.class.inc";
include_once APPRAIZ . "cproc/classes/WorkflowCproc.class.inc";
// --------------------------------------------------------------------------------------------------
include_once '../../cproc/classes/DisponibilidadeDeVistas.class.inc';

if( !verificaExistenciaPrcid() ){
	echo "
		<script>
			alert('Sessão perdeu processo.');
			window.location.href = 'cproc.php?modulo=principal/lista_grid_processo&acao=A';
		</script>";
	exit;	
}
 if ($_SESSION['cproc']['prcid']) {
         $processo = exibirProcesso($_SESSION['cproc']['prcid']);
         extract($processo);
    }
    else{
        $processo = exibirProcesso($prcid);
    }




$DisponibilidadeDeVistas = new DisponibilidadeDeVistas();

if( $_GET['dpvid'] ){

	if( $_GET['excluir'] ){
		$DisponibilidadeDeVistas->excluir( $_GET['dpvid'] );
		if( $DisponibilidadeDeVistas->commit() ){
			echo "<script>
				alert('Elemento excluído com sucesso.');
				window.location.href = 'cproc.php?modulo=principal/disponibilidadedevistas&acao=A';
			</script>";
			exit;
		}else{
			echo "<script>
				alert('Erro ao tentar excluir elemento.');
				window.history.back();
			</script>";
			exit;
		}
	}

	$DisponibilidadeDeVistas->carregar( $_GET['dpvid'] );
}

if( $_POST['tipoForm'] ):

	if( $_POST['tipoForm'] == 'filtro' ){ // ------------------------------- filtro

		// -- filtros --
		$filtros = array();
		foreach( $_POST as $chave => $valor )
			if( $valor != '' )
				switch ($chave) {
					case 'dpvservidorresponsavel_filtro':
						$filtros['dpvservidorresponsavel'] = $valor;
						break;
					case 'dpvnomesolicitante_filtro':
						$filtros['dpvnomesolicitante'] = $valor;
						break;
					case 'dpvemailsolicitante_filtro':
						$filtros['dpvemailsolicitante'] = $valor;
						break;
					case 'dpvtelefonesolicitante_filtro':
						$filtros['dpvtelefonesolicitante'] = $valor;
						break;
					case 'dpvcpfsolicitante_filtro':
						//Pega o valor do CPF do solicitante informado no filtro, sem a máscara
						$filtros['dpvcpfsolicitante'] = str_replace('-','',str_replace('.','',$valor));
						break;
					case 'dpvdatavista_filtro':
						$filtros['dpvdatavista'] = ajusta_data($valor);
						break;
					/*case 'dpvdataprevdevolucao_filtro':
						$filtros['dpvdataprevdevolucao'] = ajusta_data($valor);
						break;*/
					/*case 'dpvdatarealdevolucao_filtro':
						$filtros['dpvdatarealdevolucao'] = ajusta_data($valor);
						break;*/
				}
		// -- / filtros --

	}else if( $_POST['tipoForm']  == 'edicao' ){ // ------------------------------- edicao

		// recepcao de dados
		$campos = array();
		unset($campos['co_ies_campo']);
		unset($campos['co_mantenedora']);
		unset($campos['prcnumsidoc']);
		$campos['dpvid'] = $_POST['dpvid_campo'];
		$campos['prcid'] = $_POST['prcid_campo'];
		$campos['dpvservidorresponsavel'] = $_POST['dpvservidorresponsavel_campo'];
		$campos['dpvnomesolicitante'] = $_POST['dpvnomesolicitante_campo'];
		$campos['dpvemailsolicitante'] = $_POST['dpvemailsolicitante_campo'];
		$campos['dpvtelefonesolicitante'] = $_POST['dpvtelefonesolicitante_campo'];
		$campos['dpvcpfsolicitante'] = $_POST['dpvcpfsolicitante_campo'];
	    $campos['dpvdatavista'] = ajusta_data($_POST['dpvdatavista_campo']);
	    //$campos['dpvdataprevdevolucao'] = ajusta_data($_POST['dpvdataprevdevolucao_campo']);
	    //$campos['dpvdatarealdevolucao'] = ajusta_data($_POST['dpvdatarealdevolucao_campo']);

	    if( !$campos['dpvid'] ){ // insert

	    	// ver($campos,d);
	    	// $DisponibilidadeDeVistas->cadastrar($campos);
	    	if( $DisponibilidadeDeVistas->cadastrar($campos) ){
	    		echo "<script>
	    				alert('Cadastrado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/disponibilidadedevistas&acao=A';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo "<script>
	    				alert('Erro ao cadastrar.');
	    				window.location.href = 'cproc.php?modulo=principal/disponibilidadedevistas&acao=A';
	    			</script> ";
	    		exit;
	    	}

	    }else{ // update

	    	$DisponibilidadeDeVistas->popula( $campos );
	    	if( $DisponibilidadeDeVistas->atualizar() ){
	    		echo " <script>
	    				alert('Atualizado com sucesso.');
	    				window.location.href = 'cproc.php?modulo=principal/disponibilidadedevistas&acao=A&dpvid=".($DisponibilidadeDeVistas->dpvid)."';
	    			</script> ";
	    		exit;
	    	}else{
	    		echo " <script>
	    				alert('Existem campos obrigatórios em branco.');
	    				window.history.back();
	    			</script> ";
	    		exit;
	    	}
	    }

	}

endif;

$habil = retornaPermissaoDetalhe($prcid);

?>

<br>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript">
	jQuery.noConflict();
	
	jQuery('.adicionar').live('click',function(){
		return window.open('assint.php?modulo=principal/cadCursoPEC_G&acao=A','modelo',"height=450,width=850,scrollbars=yes,top=50,left=200");
	});
</script>

<?php 
$arMnuid = array();
$db->cria_aba( ABA_CAD_PROCESSO, $url, '', $arMnuid );
?>

<div style="width:98%;margin:0 auto;">
	<h4>Dados da Disponibilidade de Vistas:</h4>
	
	<div>

		<form method="post" name="formDisponibilidadeDeVistas">
		<input name="tipoForm" id="tipoForm" value="edicao" type="hidden"/>
		<input name="dpvid_campo" id="dpvid_campo" type="hidden" value="<?=($DisponibilidadeDeVistas->dpvid)?$DisponibilidadeDeVistas->dpvid:''?>"/>
		<input name="prcid_campo" id="prcid_campo" type="hidden" value="<?=$processo['prcid']?>"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
	        <tr>
	            <td class="subtituloDireita" colspan="2">Nº do Processo:</td>
	            <td>
	                <?php
	                    echo campo_texto('prcnumsidoc_campo', 'S', 'N', 'Nº Controle SIMEC', '51', '25', '####.######/####-##', '', '', '', '', 'id="prcid_campo"', '', $processo['prcnumsidoc'], '');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantenedora:</td>
	            <td>
	                <?php
	                    $sql = "
	                        SELECT  co_mantenedora AS codigo,
	                                no_razao_social AS descricao
	                        FROM emec.mantenedora
	                        ORDER BY no_razao_social
	                    ";
	                    $db->monta_combo('mntid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_mantenedora'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Mantida:</td>
	            <td>
	                <?php
	                   	$sql = "
	                        SELECT  co_ies AS codigo,
	                                no_ies AS descricao
	                        FROM emec.ies
	                        ORDER BY no_ies
	                    ";
	                    $db->monta_combo('iesid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['co_ies'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Entidade:</td>
	            <td>
	                <?php
	                   	$sql = "
	                        SELECT  entid AS codigo,
	                                entdsc AS descricao
	                        FROM cproc.entidade
	                        ORDER BY entdsc
	                    ";
	                    $db->monta_combo('entid', $sql, 'N', 'Selecione...', '', '', '', '455', 'S', '', '',$processo['entid'],'Tipo de Processo');
	                ?>
	            </td>
	        </tr>
	        	        	        
	        <?php responsaveisProcesso($prcid)?>
	        <tr style="height:20px;"></tr>
			<tr>
	            <td class="subtituloDireita" colspan="2">Servidor Responsável:</td>
	            <td>
	                <?php
	                	$dpvservidorresponsavel = ($DisponibilidadeDeVistas->dpvservidorresponsavel)?$DisponibilidadeDeVistas->dpvservidorresponsavel:'';
	                    echo campo_texto('dpvservidorresponsavel_campo', 'S', $habil, 'Servidor Responsável', '51', '90', '', '', '', '', '', 'id="dpvservidorresponsavel_campo"', '', $dpvservidorresponsavel, '');
	                ?>
	            </td>
	        </tr>

             <tr>
                <td class="subtituloDireita" rowspan="5" width="20%">Solicitante:</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">Nome:</td>
                <td>
                  <?php
                  		$dpvnomesolicitante = ($DisponibilidadeDeVistas->dpvnomesolicitante)?$DisponibilidadeDeVistas->dpvnomesolicitante:'';
                      	echo campo_texto('dpvnomesolicitante_campo', 'S', $habil, 'Nome Solicitante', '51', '90', '', '', '', '', '', 'id="dpvnomesolicitante_campo"', '', $dpvnomesolicitante, '');
                  ?>
              </td>
            </tr>
            <tr>
                <td class="subtituloDireita">CPF:</td>
                <td>
                  <?php
                  		$dpvcpfsolicitante = ($DisponibilidadeDeVistas->dpvcpfsolicitante)?$DisponibilidadeDeVistas->dpvcpfsolicitante:'';
                      	echo campo_texto('dpvcpfsolicitante_campo', 'S', $habil, 'CPF Solicitante', '11', '25', '###.###.###-##', '', '', '', '', 'id="dpvcpfsolicitante_campo"', '', $dpvcpfsolicitante, '');
                  ?>
      			</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">E-Mail:</td>
                <td>
                  <?php
                  		$dpvemailsolicitante = ($DisponibilidadeDeVistas->dpvemailsolicitante)?$DisponibilidadeDeVistas->dpvemailsolicitante:'';
                      	echo campo_texto('dpvemailsolicitante_campo', 'S', $habil, 'E-Mail Solicitante', '51', '50', '', '', '', '', '', 'id="dpvemailsolicitante_campo"', '', $dpvemailsolicitante, '');
                  ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Telefone:</td>
                <td>
                  <?php
                  		$dpvtelefonesolicitante = ($DisponibilidadeDeVistas->dpvtelefonesolicitante)?$DisponibilidadeDeVistas->dpvtelefonesolicitante:'';
                      	echo campo_texto('dpvtelefonesolicitante_campo', 'S', $habil, 'Telefone Solicitante', '51', '14', '', '', '', '', '', 'id="dpvtelefonesolicitante_campo"', '', $dpvtelefonesolicitante, '');
                  ?>
                </td>
            </tr>
	        
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Vista:</td>
	            <td>
	                <?php 
	                	$dpvdatavista = ($DisponibilidadeDeVistas->dpvdatavista)?$DisponibilidadeDeVistas->dpvdatavista:'';
	                	echo campo_data2('dpvdatavista_campo', 'S', $habil, 'Data Entrega', 'S', '', '', $dpvdatavista, '', '', 'dpvdatavista_campo'); 
	                ?>
	            </td>
	        </tr>
	        <!-- tr>
	            <td class="subtituloDireita" colspan="2">Data Prevista para Devolução:</td>
	            <td>
	                <?php 
	                	//$dpvdataprevdevolucao = ($DisponibilidadeDeVistas->dpvdataprevdevolucao)?$DisponibilidadeDeVistas->dpvdataprevdevolucao:'';
	                	//echo campo_data2('dpvdataprevdevolucao_campo', 'S', 'S', 'Data Prevista para Devolução', 'S', '', '', $dpvdataprevdevolucao, '', '', 'dpvdataprevdevolucao_campo'); 
	                ?>
	            </td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Real da Devolução:</td>
	            <td>
	                <?php 
	                	//$dpvdatarealdevolucao = ($DisponibilidadeDeVistas->dpvdatarealdevolucao)?$DisponibilidadeDeVistas->dpvdatarealdevolucao:'';
	                	//echo campo_data2('dpvdatarealdevolucao_campo', 'S', 'S', 'Data Real da Devolução', 'S', '', '', $dpvdatarealdevolucao, '', '', 'dpvdatarealdevolucao_campo'); 
	                ?>
	            </td>
	        </tr -->
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	            	<?php if($habil=='S'){ ?>
		                <input type="button" name="salvarProcesso" id="salvarProcesso" value="<?=( $_GET['dpvid'] )?'Salvar':'Cadastrar';?>" onclick="salvar();"/>
		                &nbsp;&nbsp;&nbsp;
		                <input name="reset" type="reset" value="Limpar campos"/>
		                <?php if( $_GET['dpvid'] ){ ?>
		                	&nbsp;&nbsp;&nbsp;
		                	<input name="novo" type="button" value="Cadastrar Nova Disponibilidade de Vistas" onclick="javascript:window.location.href='cproc.php?modulo=principal/disponibilidadedevistas&acao=A'"/>
		                <?php } ?>
	                <?php } ?>
	            </td>
	        </tr>
	    </table>
		</form>

	</div>

	<hr/>

	<div>
		<h4>Lista:</h4>

		<form method="post" name="form_filtro">
		<input name="tipoForm" id="tipoForm" value="filtro" type="hidden"/>
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" border="0">
			<tr><td colspan="3"><h5>Filtros</h5></td></tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Servidor Responsável:</td>
	            <td>
	            	<?php
	            		$dpvservidorresponsavel_filtro = ($filtros['dpvservidorresponsavel'])?$filtros['dpvservidorresponsavel']:'';
	                    echo campo_texto('dpvservidorresponsavel_filtro', 'S', 'S', 'Servidor Responsável', '51', '25', '', '', '', '', '', 'id="dpvservidorresponsavel_filtro"', '', $dpvservidorresponsavel_filtro, '');
	                ?>
	            </td>
	        </tr>
	        
            <tr>
                <td class="subtituloDireita" rowspan="5" width="20%">Solicitante:</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">Nome:</td>
                <td>
                  <?php
                  		$dpvnomesolicitante_filtro = ($filtros['dpvnomesolicitante'])?$filtros['dpvnomesolicitante']:'';
                      	echo campo_texto('dpvnomesolicitante_filtro', 'S', 'S', 'Nome Solicitante', '51', '90', '', '', '', '', '', 'id="dpvnomesolicitante_filtro"', '', $dpvnomesolicitante_filtro, '');
                  ?>
              </td>
            </tr>
            <tr>
                <td class="subtituloDireita">CPF:</td>
                <td>
                  <?php
                  		$dpvcpfsolicitante_filtro = ($filtros['dpvcpfsolicitante'])?$filtros['dpvcpfsolicitante']:'';
                      	echo campo_texto('dpvcpfsolicitante_filtro', 'S', 'S', 'CPF Solicitante', '11', '25', '###.###.###-##', '', '', '', '', 'id="dpvcpfsolicitante_filtro"', '', $dpvcpfsolicitante_filtro, '');
                  ?>
      			</td>
            </tr>
            <tr>
                <td class="subtituloDireita" width="12%">E-Mail:</td>
                <td>
                  <?php
                  		$dpvemailsolicitante_filtro = ($filtros['dpvemailsolicitante'])?$filtros['dpvemailsolicitante']:'';
                      	echo campo_texto('dpvemailsolicitante_filtro', 'S', 'S', 'E-Mail Solicitante', '51', '50', '', '', '', '', '', 'id="dpvemailsolicitante_filtro"', '', $dpvemailsolicitante, '');
                  ?>
                </td>
            </tr>
            <tr>
                <td class="subtituloDireita">Telefone:</td>
                <td>
                  <?php
                  		$dpvtelefonesolicitante_filtro = ($filtros['dpvtelefonesolicitante'])?$filtros['dpvtelefonesolicitante']:'';
                      	echo campo_texto('dpvtelefonesolicitante_filtro', 'S', 'S', 'Telefone Solicitante', '51', '14', '', '', '', '', '', 'id="dpvtelefonesolicitante_filtro"', '', $dpvtelefonesolicitante_filtro, '');
                  ?>
                </td>
            </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Vista:</td>
	            <td> 
	            	<?php 
	                	$dpvdatavista_filtro = ($filtros['dpvdatavista'])?$filtros['dpvdatavista']:'';
	                	echo campo_data2('dpvdatavista_filtro', 'S', 'S', 'Data Entrega', 'S', '', '', $dpvdatavista_filtro, '', '', 'dpvdatavista_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <!-- tr>
	            <td class="subtituloDireita" colspan="2">Data Prevista para Devolução:</td>
	            <td> 
	            	<?php 
	                	//$dpvdataprevdevolucao_filtro = ($filtros['dpvdataprevdevolucao'])?$filtros['dpvdataprevdevolucao']:'';
	                	//echo campo_data2('dpvdataprevdevolucao_filtro', 'S', 'S', 'Data Prevista para Devolução', 'S', '', '', $dpvdataprevdevolucao_filtro, '', '', 'dpvdataprevdevolucao_filtro'); 
	                ?>
	           	</td>
	        </tr>
	        <tr>
	            <td class="subtituloDireita" colspan="2">Data Real da Devolução:</td>
	            <td> 
	            	<?php 
	                	//$dpvdatarealdevolucao_filtro = ($filtros['dpvdatarealdevolucao'])?$filtros['dpvdatarealdevolucao']:'';
	                	//echo campo_data2('dpvdatarealdevolucao_filtro', 'S', 'S', 'Data Real da Devolução', 'S', '', '', $dpvdatarealdevolucao_filtro, '', '', 'dpvdatarealdevolucao_filtro'); 
	                ?>
	           	</td>
	        </tr -->
	        <tr>
	            <td class="SubTituloCentro" colspan="3">
	                <input type="submit" name="buscar" id="buscar" value="Buscar"/>
	                &nbsp;&nbsp;&nbsp;<!-- &nbsp; -->
	                <input name="reset" type="reset" value="Limpar campos"/>
	            </td>
	        </tr>
	    </table>
		</form>

		<?php 
		$cabecalho = array("Ação","Servidor Responsável","Email","Telefone","Data Vista");
		$alinhamento = array('left','left','left','left','left');
		$larguras = array('10%','30%','25%','15%','15%');
		$filtros['prcid'] = $processo['prcid'];
		// ver($DisponibilidadeDeVistas->montaListaQuery( $filtros ),d);
		$db->monta_lista($DisponibilidadeDeVistas->montaListaQuery( $filtros ),$cabecalho,30,5,'N','','N','listaDisponibilidadeDeVistas', $larguras,$alinhamento);
		?>
	</div>

</div>

<script>

function salvar(){
	document.forms.formDisponibilidadeDeVistas.submit();
}

function editar( chave ){
	window.location.href = "cproc.php?modulo=principal/disponibilidadedevistas&acao=A&dpvid="+chave;
}

function excluir( chave ){
	if( confirm('Deseja apagar esse item?') ){
		window.location.href = "cproc.php?modulo=principal/disponibilidadedevistas&acao=A&excluir=1&dpvid="+chave;
	}
}

function verDocumentosAnexados( chave ){
	popupDocumentosAnexados = window.open( "cproc.php?modulo=principal/disponibilidadedevistasdocumentosanexados&acao=A&dpvid="+chave, 'DocumentosAnexados', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
}

</script>