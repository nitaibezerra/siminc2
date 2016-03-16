<?php 

if($_POST['requisicaoAjax']){
	$_POST['requisicaoAjax']();
	die;
}

$arOrgid = verificaAcessoEmOrgid();
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}

$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid                       = $_SESSION['obras2']['orgid'];

/*
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
//
//if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
//	$_SESSION['obras2']['orgid'] = '';
//}
//$_SESSION['obras2']['orgid'] = $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
//$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
//$orgid 						 = $_SESSION['obras2']['orgid'];
*/

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

switch ( $_REQUEST['op'] ){
	case 'apagar':
		$empreendimento = new Empreendimento( $_POST['empid'] );
		if ( $empreendimento->empid ){
			$empreendimento->empstatus = 'I';
			$empreendimento->salvar();
		}
		$db->commit();
		die('<script type="text/javascript">
				alert(\'Operação realizada com sucesso!\');
				location.href=\'?modulo=principal/listaEmpreendimentos&acao=A\';
			 </script>');
	case 'abreVistoria':		
		$_SESSION['obras2']['empid'] = $_GET['empid'];
		$_SESSION['obras2']['sosid'] = $_GET['sosid'];
		$_SESSION['obras2']['sueid'] = $_GET['sueid'];
		
		if ( $_SESSION['obras2']['sueid'] ){
			header('location: ?modulo=principal/cadVistoriaEmpresa&acao=E');
		}else{
			header('location: ?modulo=principal/cadVistoriaEmpresa&acao=A');
		}
		die;
}

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgaoemprendimentoempresa');
echo montarAbasArray($arAba, "?modulo=principal/listaEmpreendimentoEmpresa&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Obras por OS', '<img align="absmiddle" src="/imagens/edit_on.gif" style="margin-right: 3px;" title=\"Ir para a Supervisão\"> Ir para a Supervisão');
//cria_abas_Obras(ID_ABA_LISTA_EMP,$url,$parametros);
//monta_titulo_listaObras();
extract( $_POST );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$('.pesquisar').click(function(){
		var op = $('#op').val();
                $('#op').val('pesquisar');
                $('#xls').val('0');
                $('#xlsparalizada').val('0');
		$('#formListaObra').submit();
                $('#op').val(op);
	});
	
	$('.exportarxls').click(function(){
		$('#xls').val('1');
        $('#xlsparalizada').val('0');
		$('#formListaObra').submit();
	});
    $('.ver-maps').click(function(){
        $('#mapa').val('1');
        $('#formListaObra').submit();
	});
    $('.exportarxlsparalizadas').click(function(){
		$('#xls').val('1');
                $('#xlsparalizada').val('1');
		$('#formListaObra').submit();
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
		$('#abreBuscaAvancada').val('t');
	}else{
		$('#tr_busca_avancada').hide();
		$('#labelBuscaAvancada').show();
		$('#abreBuscaAvancada').val('f');
	} 	
}

function carregarMunicipio( estuf ){
        var td	= $('#td_municipio');
        if ( estuf != '' ){
                var url = location.href;
                $.ajax({
                        url  		 : url,
                        type 		 : 'post',
                        data 		 : {ajax  : 'municipio', 
                                                  estuf : estuf},
                        dataType   : "html",
                        async		 : false,
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

function alterarEmp( empid ){
	location.href = '?modulo=principal/cadEmpreendimento&acao=A&empid=' + empid;
}

function abreVistoriaEmpresa( empid, sosid, sueid ){
	location.href = '?modulo=principal/listaEmpreendimentoEmpresa&acao=A&op=abreVistoria&empid=' + empid + '&sosid=' + sosid + '&sueid=' + sueid;
}

function abreObraEmpresa( obrid){
	window.open('?modulo=principal/cadObra&acao=A&visualizar=1&obrid='+obrid, 
			'ObraSupervisaoDetalhe',
			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}

function abreEmpreendimentoEmpresa( empid ){
	window.open('?modulo=principal/listaObrasEmpreendimento&acao=A&empid='+empid, 
			'ObraSupervisaoDetalhe',
			"height=640,width=970,scrollbars=yes,top=50,left=200" ).focus();
}

function imprimirLaudo( sueid , empid ){
	return windowOpen( '?modulo=principal/popupImpressaoLaudo&acao=A&sueid=' + sueid + '&empid=' + empid,'blank',
						'height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function imprimirQuestionarioRespondido(sueid, sosid){
	return windowOpen( '?modulo=principal/cadVistoriaEmpresaImpressaoPreenchido&acao=A&sueid='+sueid+'&sosid='+sosid,'blank',
			   'height=700,width=1000,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}


function abrirConsultaEmpresa( empid ){
	return windowOpen( '?modulo=principal/selecionaObjeto&acao=V&empid=' + empid, 'blank',
			   'height=900,width=1200,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

</script>
<form method="post" name="formListaObra" id="formListaObra">
	<input type="hidden" name="op" id="op" value="">
	<input type="hidden" name="xls" id="xls" value="">
	<input type="hidden" name="mapa" id="mapa" value="">
        <input type="hidden" name="xlsparalizada" id="xlsparalizada" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<input type="hidden" name="abreBuscaAvancada" id="abreBuscaAvancada" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
			<td>
				<?=campo_texto('empbuscatexto','N','S','',70,100,'','', '', '', '', 'id="empbuscatexto"', '', $_REQUEST['empbuscatexto']);?>
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
                                        <td class="SubTituloDireita" style="width: 190px;">Tipo de Obra:</td>
                                        <td>
                                            <?php 
                                            $tipoObra = new TipoObra();
                                            $db->monta_combo( "tobid", $tipoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "tobid" );
                                            ?>
                                        </td>
                                    </tr>
                                    <tr>
                                        <td class="SubTituloDireita" style="width: 190px;">Classificação da Obra:</td>
                                        <td>
                                            <?php 
                                            $classificacaoObra = new ClassificacaoObra();				
                                            $db->monta_combo( "cloid", $classificacaoObra->listaCombo(), "S", "Todos", "", "", "", 200, "N", "cloid" );
                                            ?>
                                        </td>
                                    </tr>
                             
                                    
				<?php 
				if ( $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL ):
				?>
                                    <tr>
                                        <td class="SubTituloDireita" style="width: 190px;">Tipologia da Obra:</td>
                                        <td>
                                            <?php 
                                                $tipologiaObra = new TipologiaObra();							
                                                $db->monta_combo( "tpoid", $tipologiaObra->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todas", "", "", "", 200, "N", "tpoid" );
                                            ?>
                                        </td>
                                    </tr>
				<?php
				endif;
				?>
                                <tr>
                                    <td class="SubTituloDireita" style="width: 190px;">Programa:</td>
                                    <td>
                                        <?php 
                                        $programa = new ProgramaFonte();				
                                        $db->monta_combo( "prfid", $programa->listaCombo( array("orgid" => $_SESSION['obras2']['orgid']) ), "S", "Todos", "", "", "", 200, "N", "prfid" );
                                        ?>
                                    </td>
                                </tr>
                                <?php if($_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_SUPERIOR && $_SESSION['obras2']['orgid'] != ORGID_EDUCACAO_PROFISSIONAL): ?>
				<tr>
                                    <td class="SubTituloDireita" style="width: 190px;">Modalidade de Ensino:</td>
                                    <td>
                                        <?php 
                                        $modalidade = new ModalidadeEnsino();
                                        $db->monta_combo( "moeid", $modalidade->listaCombo(), "S", "Todos", "", "", "", 200, "N", "moeid" );
                                        ?>
                                    </td>
				</tr>
                                <?php endif; ?>
			
				<tr>
                                    <td class="SubTituloDireita">UF:</td>
                                    <td>
                                        <?php
                                        $uf = new Estado();					
                                        $db->monta_combo("estuf", $uf->listaCombo(), 'S','Selecione...','carregarMunicipio', '', '',200,'N','estuf'); 
                                        ?>
                                    </td>
				</tr>
				<tr>
                                    <td class="SubTituloDireita">Município:</td>
                                    <td id="td_municipio">
                                    <?php 
                                        if ($estuf){
                                                $municipio = new Municipio();
                                                $dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
                                                $habMun    = 'S';
                                        }else{
                                                $dado   = array();
                                                $habMun = 'N';
                                        }		
                                        $habMun = ($disable == 'N' ? $disable : $habMun);
                                        echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod'); 
                                    ?>
                                    </td>
				</tr>
				<tr>
                                    <td class="SubTituloDireita">Esfera:</td>
                                    <td>
                                        <?php 
                                        $sql = Array(Array('codigo'=>'M', 'descricao'=>'Municipal'),
                                                                 Array('codigo'=>'E', 'descricao'=>'Estadual'),
                                                                 Array('codigo'=>'F', 'descricao'=>'Federal'));
                                        $db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
                                        ?>
                                    </td>
				</tr>
				<tr>
                                    <td class="SubTituloDireita" style="width: 190px;">Valor da Obra:</td>
                                    <td>
                                        De:&nbsp;
                                        <?php 
                                            echo campo_texto( 'empvalorprevisto_menor', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                            echo 'Até:&nbsp;';
                                            echo campo_texto( 'empvalorprevisto_maior', 'N', 'S', '', 11, 30, '[###.]###,##', '', 'right', '', 0, '');
                                        ?>
                                    </td>
				</tr>
                            </table>
			</td>
		</tr>
		<!-- Acrescentando mais filtros -->
		<tr>
                    <td class="SubTituloDireita">Número da OS:</td>
                    <td>
                        <?php
                            echo campo_texto( 'sosnum', 'N', 'S', '', 11, 30, '###########', '', 'right', '', 0, '');
                        ?>
                    </td>
		</tr>
		<tr>
                    <td class="SubTituloDireita">Situação da OS:</td>
                    <td>
                        <?php
                            criaComboWorkflow(TPDID_OS,array("nome"=>"esdidsituaco","id"=>"esdidsituacao"));
                        ?>
                    </td>
		</tr>
                
                 <tr>
		            <td class="SubTituloDireita">Situação da Obra na Supervisão</td>
		            <td>
		                <?php
				$situacaoObra = new SituacaoObra();
		                $sql = $situacaoObra->listaCombo();
		                $db->monta_combo("sobid", $sql, "S", "Selecione...", "ctrlSituacaoObra", '', '', '', 'N', 'sobid');
		                ?>
		            </td>
		</tr>
			<?php
				$param = array("estuf"  => array( "obrigatorio"=>"N", "value"    => $_POST['obraestuf']),
                                               "mescod" => array( "obrigatorio"=>"N", "disabled" => "S", "value"=>$_POST['obramescod']),
                                               "miccod" => array( "disabled"=>"S",    "value"    => $_POST['obramiccod']),
                                               "muncod" => array( "disabled"=>"S",    "value"    => $_POST['obramuncod'])); 
				montaComboTerritorio("obra", $param);
			 ?>
		<tr>
                    <td class="SubTituloDireita">Empresa responsável</td>
                    <td>
                        <?php
                            $supervisao_Grupo_Empresa = new Supervisao_Grupo_Empresa();
                            $empresas = $supervisao_Grupo_Empresa->listaComboEntidades();
                            echo $db->monta_combo("entid", $empresas, 'S','Selecione...','', '', '',200,'N','sgeid'); 
                        ?>
                    </td>
		</tr>
		<tr>
                    <td class="SubTituloDireita">Situação da supervisão:</td>
                    <td>
                        <?php
                            criaComboWorkflow(WF_TPDID_LAUDO_SUPERVISAO_EMPRESA,array("nome"=>"esdidsupervisao","id"=>"esdidsupervisao","option" => array("nao_iniciado"=>"Não Iniciado")), true);
                        ?>
                    </td>
		</tr>
                      
		<tr>
                    <td class="SubTituloDireita">Possui supervisão atrasada?</td>
                    <td>
                        <input type="radio" id="atrasado_sim" <?php echo $_POST['atrasado'] == "sim" ? "checked='checked'" : "" ?> name="atrasado" value="sim"/>   Sim
                        <input type="radio" id="atrasado_nao" <?php echo $_POST['atrasado'] == "nao" ? "checked='checked'" : "" ?> name="atrasado" value="nao"/>   Não
                        <input type="radio" id="atrasado_todos" <?php echo $_POST['atrasado'] == "todos" ? "checked='checked'" : "" ?> name="atrasado" value="todos"/> Todas
                    </td>
		</tr>
        <tr>
            <td class="SubTituloDireita">Supervisão homologada com atraso</td>
            <td>
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "sim" ? "checked='checked'" : "" ?> value='sim' />Sim
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "nao" ? "checked='checked'" : "" ?> value='nao' />Não
                <input type='radio' name='h_atrasada' <?php echo $_POST['h_atrasada'] == "todas" ? "checked='checked'" : "" ?> value='todas' />Todas
            </td>
        </tr>
		<tr>
                    <td class="SubTituloDireita">No cadastro da obra o endereço está correto?</td>
                    <td>
                        <input type="radio" id="endereco_obr_sim"   name="endereco_obr" value="sim"   <?php echo $_POST['endereco_obr'] == "sim" ? "checked='checked'" : "" ?> />   Sim
                        <input type="radio" id="endereco_obr_nao"   name="endereco_obr" value="nao"   <?php echo $_POST['endereco_obr'] == "nao" ? "checked='checked'" : "" ?> />   Não
                        <input type="radio" id="endereco_obr_todos" name="endereco_obr" value="todos" <?php echo $_POST['endereco_obr'] == "todos" ? "checked='checked'" : "" ?> /> Todas
                    </td>
		</tr>
        <tr>
            <td class="SubTituloDireita" style="width: 190px;">Obras MI?</td>
            <td>
                <input type="radio" name="obrami" id="" value="S" <?= ( $_POST["obrami"] == "S" ? "checked='checked'" : "" ) ?>/> Sim
                <input type="radio" name="obrami" id="" value="N" <?= ( $_POST["obrami"] == "N" ? "checked='checked'" : "" ) ?> /> Não
                <input type="radio" name="obrami" id="" value="T" <?= ( $_POST["obrami"] == "T" || $_POST["obrami"] == "" ? "checked='checked'" : "" ) ?>/> Todas
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita" valign="top">Tipo de Supervisão</td>
            <td colspan="3">
                <input type="radio" name="sosterreno" id="" value="f" <?= ($_POST["sosterreno"] == 'f') ? 'checked="checked"' : '' ?> > Obra
                <input type="radio" name="sosterreno" id="" value="t"  <?= ($_POST["sosterreno"] == 't') ? 'checked="checked"' : '' ?> > Implantação de Obra
                <input type="radio" name="sosterreno" id="" value=""  <?= (empty($_POST["sosterreno"])) ? 'checked="checked"' : '' ?> > Todas
            </td>
        </tr>

        <tr>
                    <td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
                        <input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                        <input type="button" name="btn_todas" value="Ver Todas" onclick="window.location.href=window.location" />
                        <? if(possui_perfil( array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC) )):?>
                        <input type="button" name="ver-maps" value="Mapa" class="ver-maps" />
                        <? endif; ?>
                        <input type="button" name="exportar_para_xls" value="Exportar XLS" class="exportarxls" />
                        <input type="button" name="exportar_para_xls_paralizadas" value="Exportar XLS Paralisadas" class="exportarxlsparalizadas" />
                    </td>
		</tr>

	</table>
</form>
<?php 

$empreendimento = new Empreendimento();
$sql 		= $empreendimento->listaEmpreendimentoOS($_POST);
//ver(simec_htmlentities($sql));
if($_POST['xls']) {
    
    if($_POST['xlsparalizada']){
        ob_clean();
	$cabecalho = array("ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão", "Percentual Supervisão", "Obra MI", "Data Homologação","Tipo de paralização", "Observações"," O município tem interesse em finalizar a obra? ","Dentro de que prazo o município irá concluir a obra? (N° de dias) "," O município já elaborou o novo processo licitatório para reinício e conclusão da obra, se sim, qual o prazo de encerramento, se não, qual o prazo de lançamento do Edital? (N° de dias) ", "O município irá concluir a obra por administração direta? "," O município precisa de orientação sobre como proceder para der continuidade aos serviços? ", "No cadastro da obra o endereço está correto?", "CEP (supervisão)","Logradouro (supervisão)","Número (supervisão)","Complemento (supervisão)","Município (supervisão)","UF (supervisão)", "Latitude (supervisão)", "Longitude (supervisão)", "Responsável", "CEP (original)","Logradouro (original)","Número (original)","Complemento (original)","Município (original)","UF (original)", "Latitude (original)", "Longitude (original)");
	$db->sql_to_xml_excel($sql,'obras_os',$cabecalho);
	die;
        
    }else{
    ob_clean();
	$cabecalho = array("ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão", "Percentual Supervisão", "Obra MI", "Data Homologação", "No cadastro da obra o endereço está correto?", "CEP (supervisão)","Logradouro (supervisão)","Número (supervisão)","Complemento (supervisão)","Município (supervisão)","UF (supervisão)", "Latitude (supervisão)", "Longitude (supervisão)", "Responsável", "CEP (original)","Logradouro (original)","Número (original)","Complemento (original)","Município (original)","UF (original)", "Latitude (original)", "Longitude (original)");
	$db->sql_to_xml_excel($sql,'obras_os',$cabecalho);
	die;
    }
} elseif($_POST['mapa']) {
    exibeMapa($sql);
} elseif($_POST['op'] == 'pesquisar') {
	$cabecalho = array("Ação","ID Obra", "Obra", "Situação da Obra", "Empresa responsável","Nº OS","Data Fim da Execução" ,"Situação da OS", "UF - Município", "Microrregião", "Mesorregião", "Situação da Supervisão", "Data da Última Atualização da Supervisão da Empresa","Data da Supervisão","Percentual Supervisão", "Obra MI", "Data Homologação");
	$db->monta_lista($sql,$cabecalho,100,5,'N','center','');
}


function exibeMapa($sql)
{
    global $db;
    $obras = $db->carregar($sql);
    ?>
    <script type="text/javascript" src="https://maps.googleapis.com/maps/api/js?sensor=false"></script>
    <script type="text/javascript" src="/includes/maps/markerclusterer.js"></script>
    <script type="text/javascript">

        window.mapa.obras = <?=simec_json_encode($obras);?>

        window.mapa.markers = [];
        $(function(){
            var latlng = new google.maps.LatLng(-14.689881, -52.373047);
            var myOptions = {
                zoom: 4,
                center: latlng,
                mapTypeId: google.maps.MapTypeId.ROADMAP
            }
            window.mapa.map = new google.maps.Map(document.getElementById("map_canvas"), myOptions);

            $.each(window.mapa.obras, function(i){
                latlng = new google.maps.LatLng($(this)[0].latitude,$(this)[0].longitude);
                marker = new google.maps.Marker({
                    position: latlng,
                    map: window.mapa.map,
                    title: $(this)[0].uf_muni
                });
                window.mapa.markers.push(marker);
            });

            var mc = new MarkerClusterer(window.mapa.map, window.mapa.markers);
        });

    </script>
    <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
        <tr>
            <td>
                <div id="map_canvas" style="width: 100%; height: 600px"></div>
            </td>
        </tr>
    </table>

    <?

}

?>