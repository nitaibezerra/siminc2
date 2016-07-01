<?php 

if($_REQUEST['requisicao'] == 'carregaUniversidades'){

	$sql = "select 
				uniid as codigo,
				uninome as descricao
			from maismedicos.universidade
			".($_GET["q"] ? "WHERE uninome ILIKE '%{$_GET["q"]}%'" : "")."";

	$arDados = $db->carregar($sql);

	$q = strtolower($_GET["q"]);
	
	if($arDados){
		foreach ($arDados as $key=>$value){
			echo "{$value['descricao']}|{$value['codigo']}\n";
		}
	}else{
		echo "Sem registros|";
	}
	die;
}

if($_REQUEST['requisicao'] == 'reabrirLote'){
	$sql  = "update maismedicos.autorizacaopagamento set apgstatus = 'I' where lcbid in (select lcbid from maismedicos.lotecomplementarbolsista where locid = {$_REQUEST['locid']});";
	$sql .= "update maismedicos.lotecomplementar set locdatafechamento = null where locid = {$_REQUEST['locid']};";
	$db->executar($sql);
	if($db->commit()){
		$db->sucesso('principal/loteComplementar');
	}
	$db->insucesso('Falha ao reabrir lote, tente novamente!', '', 'principal/loteComplementar');
	die;
}

if($_REQUEST['requisicao']=='gerarExcelLote'){
	
	$sql = "select 
				uninome as instituicao,
				lcbtutorsolicitante as tutor_solicitante,
				lcbcpftutorsolicitante as cpf_tutor,
				lcbnomesupervisor as nome_supervisor,
				lcbcpfsupervisor as cpf_supervisor,
				case when lcbfuncao = 'S' then 'Supervisor' else 'Tutor' end as funcao,
				case when lcbbolsavalidada = 't' then 'Sim' else 'Não' end as bolsa_validada,
				case when lcbrelatoriopostado = 't' then 'Sim' else 'Não' end as relatorio_postado,
				lcbmesreferencia as mes_referencia,
				lcbanoreferencia as ano_referencia,
				lcbdataregistro as data_cadastro
			from maismedicos.lotecomplementarbolsista a
			left join maismedicos.universidade b on a.uniid = b.uniid
			where lcbstatus = 'A' 
			and locid = {$_REQUEST['locid']}";
	
	gerar_excel($sql, null, 'LOTE_COMPLEMENTAR_'.str_pad($_REQUEST['locid'], 6, '0', STR_PAD_LEFT));
	die;
}

if($_REQUEST['requisicao']=='verificaExisteBolsista'){
	$cpf = str_replace(array('.','-'), '', $_REQUEST['cpf']);
	$sql = "select tutnome, tutcpf from maismedicos.tutor where tutcpf = '{$cpf}' and tuttipo = '{$_REQUEST['funcao']}' and tutstatus = 'A';";	
	$rs = $db->pegaLinha($sql);
	$rs['tutnome'] = utf8_encode($rs['tutnome']);
	echo json_encode($rs);
	die;
}

if($_REQUEST['requisicao']=='excluirBolsista'){
	
	$arqid = $db->pegaUm("select arqid from maismedicos.lotecomplementarbolsista where lcbid = {$_REQUEST['lcbid']}");
	
	$sql  = "update maismedicos.lotecomplementarbolsista set lcbstatus = 'I' where lcbid = {$_REQUEST['lcbid']};";
	$sql .= "update maismedicos.autorizacaopagamento set apgstatus = 'I' where lcbid = {$_REQUEST['lcbid']};";
	$db->executar($sql);
	
	if($arqid){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
		$file = new FilesSimec("lotecomplementarbolsista", array(''), "maismedicos");
		$file->setRemoveUpload($arqid);
	}
	
	if($db->commit()){
		$db->sucesso('principal/loteComplementar');
	}
	
	$db->insucesso('Falha ao excluir bolsista, tente novamente!','','principal/loteComplementar');
	die;
}

if($_REQUEST['requisicao']=='pegarBolsista'){
	
	$sql = "select 
				lcbid, 
				lcbinstituicao,
				lcbtutorsolicitante,
				lcbcpftutorsolicitante,
				lcbnomesupervisor,
				lcbcpfsupervisor,
				lcbfuncao,
				lcbbolsavalidada,
				lcbrelatoriopostado,
				lcbmesreferencia,
				lcbanoreferencia,
				a.uniid,
				lcbbolsavalidada,
				lcbrelatoriopostado,
				b.uninome
			from maismedicos.lotecomplementarbolsista a
			left join maismedicos.universidade b on a.uniid = b.uniid 
			where lcbid = {$_REQUEST['lcbid']}";
	
	$rs = $db->pegaLinha($sql);
	$rs['uninome'] = utf8_encode($rs['uninome']); 
	$rs['lcbtutorsolicitante'] = utf8_encode($rs['lcbtutorsolicitante']); 
	$rs['lcbnomesupervisor'] = utf8_encode($rs['lcbnomesupervisor']); 
	echo json_encode($rs);
	die;
}

if($_REQUEST['requisicao']=='fecharLote'){
	
	$locid = $_REQUEST['locid'];
	
	$sql = "update maismedicos.lotecomplementar set locdatafechamento = now() where locid = {$locid};";
	$db->executar($sql);
	
	$sql = "select * from maismedicos.lotecomplementarbolsista where lcbstatus = 'A' and locid = {$locid};";
	$rs = $db->carregar($sql);
	
	if($rs){
		foreach($rs as $bolsista){
			$arBosistas[] = "(
						'{$bolsista['lcbcpfsupervisor']}',
						'{$bolsista['lcbnomesupervisor']}',
						'{$bolsista['lcbcpftutorsolicitante']}',
						'{$bolsista['lcbtutorsolicitante']}',
						'{$bolsista['lcbfuncao']}',
						'{$bolsista['lcbmesreferencia']}',
						'{$bolsista['lcbanoreferencia']}',						
						'A',
						'Lote complementar',
						'f',
						{$bolsista['lcbid']}						
					)";
		}
		
		$sql = "insert into maismedicos.autorizacaopagamento
				(apgcpf, apgnome, apgcpfresponsavel, apgnomeresponsavel, apgtipo, apgmes, apgano, apgstatus,apgjustificativa, apgenviado, lcbid)
			values ".implode(',', $arBosistas);
		
		$db->executar($sql);
	}
	
	
	
	if($db->commit()){
		$db->sucesso('principal/loteComplementar');
	}
	$db->insucesso('Falha ao fechar o lote, tente novamente!','','principal/loteComplementar');
}

if($_REQUEST['requisicao'] == 'salvarBolsista'){
	
	$lcbid = $_REQUEST['lcbid'];
	$locid = $_REQUEST['locid'];
	$lcbinstituicao = $_REQUEST['lcbinstituicao'];
	$lcbtutorsolicitante = $_REQUEST['lcbtutorsolicitante'];
	$lcbcpftutorsolicitante = str_replace(array('.','-'), '', $_REQUEST['lcbcpftutorsolicitante']);
	$lcbnomesupervisor = $_REQUEST['lcbnomesupervisor'];
	$lcbcpfsupervisor = str_replace(array('.','-'), '', $_REQUEST['lcbcpfsupervisor']);
	$lcbfuncao = $_REQUEST['lcbfuncao'];
	$lcbbolsavalidada = $_REQUEST['lcbbolsavalidada'];
	$lcbrelatoriopostado = $_REQUEST['lcbrelatoriopostado'];
	$lcbmesreferencia = str_pad($_REQUEST['lcbmesreferencia'],2,'0', STR_PAD_LEFT);
	$lcbanoreferencia = $_REQUEST['lcbanoreferencia'];	
	$uniid = $_REQUEST['uniid'] ? $_REQUEST['uniid'] : 'null';
	
	if($lcbid>0){
	
			$sql = "update maismedicos.lotecomplementarbolsista set 
						lcbinstituicao = '{$lcbinstituicao}', 
						lcbtutorsolicitante = '{$lcbtutorsolicitante}', 
						lcbcpftutorsolicitante = '{$lcbcpftutorsolicitante}', 
						lcbnomesupervisor = '{$lcbnomesupervisor}', 
						lcbcpfsupervisor = '{$lcbcpfsupervisor}', 
						lcbfuncao = '{$lcbfuncao}',
						lcbbolsavalidada = '{$lcbbolsavalidada}', 
						lcbrelatoriopostado = '{$lcbrelatoriopostado}', 
						lcbmesreferencia = '{$lcbmesreferencia}', 
						lcbanoreferencia = '{$lcbanoreferencia}',
						uniid = {$uniid}
					where lcbid = {$lcbid};";
			
			$db->executar($sql);
			
			if($_FILES['arquivo']['name']){
				$arqid = $db->pegaUm("select arqid from maismedicos.lotecomplementarbolsista where lcbid = {$lcbid}");
				
				if($arqid){
					include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
					$file = new FilesSimec("lotecomplementarbolsista", array(''), "maismedicos");
					$file->setPulaTableEschema(true);				
					$file->setRemoveUpload($arqid);
				}
			}
		
	}else{
		
		$sql = "insert into maismedicos.lotecomplementarbolsista
					(locid, lcbinstituicao, lcbtutorsolicitante, lcbcpftutorsolicitante, lcbnomesupervisor, lcbcpfsupervisor, lcbfuncao,
					lcbbolsavalidada, lcbrelatoriopostado, lcbmesreferencia, lcbanoreferencia, uniid)
				values
					('{$locid}','{$lcbinstituicao}','{$lcbtutorsolicitante}','{$lcbcpftutorsolicitante}','{$lcbnomesupervisor}',
					'{$lcbcpfsupervisor}','{$lcbfuncao}','{$lcbbolsavalidada}','{$lcbrelatoriopostado}','{$lcbmesreferencia}','{$lcbanoreferencia}',
					{$uniid}) returning lcbid;";
					
		$lcbid = $db->pegaUm($sql);
		
	}
	
	if($_FILES['arquivo']['name']){
		include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	
		$arrCampos = array("lcbid" => $lcbid);
		$file = new FilesSimec("lotecomplementarbolsista", $arrCampos, "maismedicos");
		$file->setUpload(null, "arquivo", false);
		$arqid = $file->getIdArquivo();
		
		$sql = "update maismedicos.lotecomplementarbolsista set arqid = {$arqid} where lcbid = {$lcbid};";
		$db->executar($sql);
		
	}
	
	if($db->commit()){
		$db->sucesso('principal/loteComplementar');
	}
	$db->insucesso('Falha ao incluir bolsista, tente novamente!','','principal/loteComplementar');
	die;
}

if($_REQUEST['requisicao']=='abrirLote'){
	
	$_REQUEST['mes'] = str_pad(trim($_REQUEST['mes']), 2, '0', STR_PAD_LEFT);
	
	$sql = "select 
				true 
			from maismedicos.lotecomplementar 
			where locstatus = 'A'
			and locmespagamento = '{$_REQUEST['mes']}'
			and locanopagamento = '{$_REQUEST['ano']}'";
	
	if($db->pegaUm($sql)){
		echo '2';
		die;
	}
	
	$sql = "insert into maismedicos.lotecomplementar
				(loccpfresponsavel, locmespagamento, locanopagamento)
			values
				('{$_SESSION['usucpf']}', '{$_REQUEST['mes']}', '{$_REQUEST['ano']}');";
	
	$db->executar($sql);
	$db->commit();
	echo '1';
	die;	
}

if($_REQUEST['requisicao'] == 'mudarMesPagamento'){
	
	if($_REQUEST['verificaExisteMes']){
		$sql = "select true from maismedicos.lotecomplementar where lpad(locmespagamento::varchar, 2, '0') = '".str_pad($_REQUEST['mes'], 2, '0', STR_PAD_LEFT)."' and locanopagamento = '{$_REQUEST['ano']}' and locstatus = 'A' and locid not in ( {$_REQUEST['locid']} )";
		$rs = $db->pegaUm($sql);
		if($rs){
			echo 'Já existe um lote complementar para o mês '.str_pad($_REQUEST['mes'], 2, '0', STR_PAD_LEFT).'/'.$_REQUEST['ano'];
		}else{
			$sql = "update maismedicos.lotecomplementar set locmespagamento = '{$_REQUEST['mes']}', locanopagamento = '{$_REQUEST['ano']}' where locid = {$_REQUEST['locid']};";
			$db->executar($sql);
			$db->commit();
			echo '1';
		}
		die;
	}
	
	$sql = "select locid, locmespagamento, locanopagamento from maismedicos.lotecomplementar where locid = {$_REQUEST['locid']}";
	$rs = $db->pegaLinha($sql);
	
	echo "<script type=\"text/javascript\" src=\"../includes/JQuery/jquery-1.4.2.js\"></script>
		  <script>
			
			$(function(){
				$('.btnMudarMesPagamento').click(function(){
					
					mes = $('[name=locmespagamentonew]').val();
					ano = $('[name=locanopagamentonew]').val();
					
					$.ajax({
						url		: 'maismedicos.php?modulo=principal/loteComplementar&acao=A',
						type	: 'post',
						data	: 'requisicao=mudarMesPagamento&verificaExisteMes=true&locid={$rs['locid']}&mes='+mes+'&ano='+ano,
						success	: function(e){
							if(e=='1'){
								alert('Operação realizada com sucesso!');
								window.opener.location.reload(true);
            					window.close();
								
							}else{
								alert(e);
							}
						}

					});
				});
			});
				
			</script>";
	
	echo '<center>';
	echo '<h3>Mudar Mês de Pagamento</h3>';
	
	$a=0;
	for($x=1;$x<13;$x++){
		$meses_periodo[$a]['codigo'] = str_pad($x, 2, '0', STR_PAD_LEFT);
		$meses_periodo[$a]['descricao'] = mes_extenso($x);
		$a++;
	}
	
	$locmespagamentonew = $rs['locmespagamento'];
	$db->monta_combo('locmespagamentonew',$meses_periodo,'S','Mês',null,null,null,null,'N','locmespagamentonew');
	
	$b=0;
	for($y=date('Y');$y<date('Y')+2;$y++){
		$anos_periodo[$b]['codigo'] = $y;
		$anos_periodo[$b]['descricao'] = $y;
		$b++;
	}
	
	$locanopagamentonew = $rs['locanopagamento'];
	$db->monta_combo('locanopagamentonew',$anos_periodo,'S','Ano',null,null,null,null,'N','locanopagamentonew');
	
	echo '<p><input type="button" value="Salvar" class="btnMudarMesPagamento" id="'.$rs['locid'].'" />';
	
	echo '</center>';
	die;
}

if($_REQUEST['requisicao'] == 'verificaPagamentoMesBolsista'){
	
	if($_REQUEST['ano'] && $_REQUEST['mes'] && $_REQUEST['cpf']){
		
		$_REQUEST['mes'] = str_pad($_REQUEST['mes'], 2, '0', STR_PAD_LEFT);
		$_REQUEST['cpf'] = str_replace(array('.', '-'), '', $_REQUEST['cpf']);
		
		$sql = "select true from maismedicos.remessadetalhe where cs_ocorrencia = '0000' and to_char(to_date(dt_ini_periodo, 'YYYYMMDD'), 'YYYYMM') = '{$_REQUEST['ano']}{$_REQUEST['mes']}' and nu_cpf = '{$_REQUEST['cpf']}'";
		$rs = $db->pegaUm($sql);
		if($rs){
			echo '1';
		}
	}
	die;
}

if($_REQUEST['requisicao']=='downloadAnexo'){
	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$file = new FilesSimec("lotecomplementarbolsista", array(''), "maismedicos");
	$file->getDownloadArquivo($_REQUEST['arqid']);
	die;
}

require_once APPRAIZ . "www/includes/webservice/cpf.php";
require_once APPRAIZ . "includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, $parametros);

$sql = "select
			*
		from maismedicos.lotecomplementar
		where locstatus = 'A'
		and locdatafechamento is null";

$rs = $db->pegaLinha($sql);

$subTitulo = ($rs['locid']>0 ? '<a href="javascript:void(0)" id="'.$rs['locid'].'" class="mudarMesPagamento">'.str_pad(trim($rs['locmespagamento']), 2, '0', STR_PAD_LEFT)."/".$rs['locanopagamento'].'</a>' : "Selecione o mês e ano para pagamento.");
monta_titulo( "Lote Complementar", $subTitulo);

$habilitado = "S";

?>

<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />

<!-- <script type="text/javascript" src="/includes/JQuery/jquery-1.10.2.min.js"></script> -->

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="js/jquery.livequery.js"></script>

<script type="text/javascript" src="/includes/jquery-validate/jquery.validate.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript" src="/includes/jquery-validate/lib/jquery.metadata.js"></script>

<script type="text/javascript" src="/includes/jquery-autocomplete/jquery.autocomplete.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/jquery-autocomplete/jquery.autocomplete.css" />

<script>
	$(function(){

		var camposObrigatorios  = "[name=lcbinstituicao], [name=lcbtutorsolicitante], [name=lcbcpftutorsolicitante], [name=lcbnomesupervisor], [name=lcbcpfsupervisor], [name=lcbfuncao], ";
		    camposObrigatorios += "[name=lcbmesreferencia], [name=lcbanoreferencia], [name=lcbbolsavalidada], [name=lcbrelatoriopostado], [name=uniid_auto]";
		    
        $(camposObrigatorios).addClass("required");

        $("#formulario").validate();

		$("[name=abrirlote]").click(function(){
			if($(this).val()=='s'){
				$('#mes_ano_referencia').show();
			}else{
				$('#mes_ano_referencia').hide();
			}
		});

		$('#btnCriaLote').click(function(){

			mes = $('[name=locmespagamento]').val();
			ano = $('[name=locanopagamento]').val();

			$.ajax({
				url		: '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A',
				type	: 'post',
				data	: 'requisicao=abrirLote&mes='+mes+'&ano='+ano,
				success	: function(e){
					if(e=='1'){
						location.reload();
					}else{
						alert('Já existe um lote complementar para o mês '+(mes<9?'0'+mes:mes)+'/'+ano+'.');
					}
				}
			});
			
		});

 		$('#btnFechaLote').click(function(){
 	 		if(confirm('Deseja fechar o lote e enviar para pagamento?')){
	 			locid = $('[name=locid]').val(); 			
				document.location.href = '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=fecharLote&locid='+locid;
 	 		}
 		});

		$('.btnEditarBolsista').click(function(){

			lcbid = this.id.replace('editar_', '');
			$.ajax({
				url		 : '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A',
				type	 : 'post',
				dataType : "json",
				data	 : 'requisicao=pegarBolsista&lcbid='+lcbid,
				success	 : function(e){
										
					$('[name=lcbid]').val(e.lcbid);
					$('[name=lcbinstituicao]').val(e.lcbinstituicao);
					$('[name=lcbtutorsolicitante]').val(e.lcbtutorsolicitante);
					$('[name=lcbcpftutorsolicitante]').val(e.lcbcpftutorsolicitante);
					$('[name=lcbnomesupervisor]').val(e.lcbnomesupervisor);
					$('[name=lcbcpfsupervisor]').val(e.lcbcpfsupervisor);
					$('[name=lcbfuncao]').val(e.lcbfuncao);
					$('[name=lcbbolsavalidada]').val(e.lcbbolsavalidada);
					$('[name=lcbrelatoriopostado]').val(e.lcbrelatoriopostado);
					$('[name=lcbmesreferencia]').val(e.lcbmesreferencia);
					$('[name=lcbanoreferencia]').val(e.lcbanoreferencia);
					$('[name=uniid]').val(e.uniid);
					$('[name=uniid_auto]').val(e.uninome);

					if(e.lcbbolsavalidada=='t'){
						$('#lcbbolsavalidada_t').attr('checked', true);
						$('#lcbbolsavalidada_f').attr('checked', false);
					}else{
						$('#lcbbolsavalidada_f').attr('checked', true);
						$('#lcbbolsavalidada_t').attr('checked', false);
					}

					if(e.lcbrelatoriopostado=='t'){
						$('#lcbrelatoriopostado_t').attr('checked', true);
					}else{
						$('#lcbrelatoriopostado_f').attr('checked', true);
					}
					
					$('#btnAddBolsista').val('Salvar bolsista');
					$("#formulario").validate().resetForm();
					$('#btnNewBolsista').show();

				}
			});
		});

		$('#btnAddBolsista').click(function(){
			
			mes = $('[name=lcbmesreferencia]').val();
			ano = $('[name=lcbanoreferencia]').val();
			funcao = $('[name=lcbfuncao]').val();
			cpf = $('[name=lcbcpfsupervisor]').val();
			
			params = '&mes='+mes+'&ano='+ano+'&funcao='+funcao+'&cpf='+cpf;
			
			$.ajax({
				url		: '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A',
				type	: 'post',
				data	: 'requisicao=verificaPagamentoMesBolsista'+params,
				success	: function(e){
					if(e=='1'){
						alert('Este bolsista já tem um pagamento para o mês'+mes+'/'+ano+'!');
					}
					$("#formulario").submit();
				}
			});
			
		});
		

		$('#btnNewBolsista').click(function(){
			
			$("#formulario").find(":input, select").each(function () {
		        switch (this.type) {
		            case "file":
		            case "password":
		            case "text":
		            	$(this).val("");
		            case "textarea":
		                $(this).val("");
		                break;
		            case "checkbox":
		            	this.checked = false;
		            case "radio":
		                this.checked = false;
		        }

		        $(this).children("option:selected").removeAttr("selected").end()
		               .children("option:first").attr("selected", "selected");
		    });
		    
			$("#formulario").validate().resetForm();			
		});
		
		$('.btnExcluirBolsista').click(function(){

			if(confirm("Deseja excluir o registro?")){
			
				lcbid = this.id.replace('excluir_','');
				document.location.href = '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=excluirBolsista&lcbid='+lcbid;
			}
		});

		$('.gerarExcelLote').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=gerarExcelLote&locid='+this.id;
		});

		$('[name=lcbcpfsupervisor]').change(function(){

			if(!$('[name=lcbfuncao]').val()){
				alert('Escolha uma função para o bolsista antes de digitar o CPF!');
				$(this).val('');
				$('[name=lcbfuncao]').focus();
				return false;
			}
			
			cpf = this.value;
			funcao = $('[name=lcbfuncao]').val();
			
			$.ajax({
				url		 : '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A',
				type	 : 'post',
				dataType : "json",
				data	 : 'requisicao=verificaExisteBolsista&cpf='+cpf+'&funcao='+funcao,
				success	 : function(e){
					if(typeof e.tutnome == 'undefined'){
						alert('O supervisor não foi localizado em nossa base de dados!');
					}else if(e.tutnome == '' || e.tutnome == 'null' || e.tutnome == null){
						alert('Nulo');
					}else{
						$('[name=lcbnomesupervisor]').val(e.tutnome);
						$('[name=lcbnomesupervisor]').attr("readonly","readonly");
					}
				}
			});
		});

		$(".reabrirLote").click(function(){
			if(confirm("Deseja reabrir o lote complementar?")){
				document.location.href = '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=reabrirLote&locid='+this.id;
			}	
		});

		$("[name=uniid_auto]").autocomplete('maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=carregaUniversidades', {			
			matchContains: true,
			minChars: 0, 			
			cacheLength:1000,
			width: 440,
			autoFill: false,
			max: 1000    		
		}).result(function(event, data, formatted) {
			var uniid = data[1];
			$('[name=uniid]').val(uniid);
		});

		$('.mudarMesPagamento').click(function(){
			var popup = window.open( 'maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=mudarMesPagamento&locid='+this.id, 'mudarMesPagamento', "height=180,width=400,scrollbars=yes,top=250,left=200" );
			popup.focus();
		});

		$('.btnAbrirAnexo').click(function(){
			document.location.href = '/maismedicos/maismedicos.php?modulo=principal/loteComplementar&acao=A&requisicao=downloadAnexo&arqid='+this.id;
		});

		$('[name=lcbcpftutorsolicitante]').change(function(){
			
			if( !validar_cpf( $(this).val()  ) ){
				alert( "CPF inválido!\nFavor informar um cpf válido!" );
				$(this).val('');
				$('[name=lcbtutorsolicitante]').val('');
				return false;	
			}
			
			var comp  = new dCPF();
			comp.buscarDados( $(this).val() );
			if (comp.dados.no_pessoa_rf != ''){
				$('[name=lcbtutorsolicitante]').val(comp.dados.no_pessoa_rf);				
				$('[name=lcbtutorsolicitante]').attr("readonly","readonly");
			}

		});

	});
	
</script>

<?php if($rs['locid']>0): ?>

	<center>
	
		<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data">

			<input type="hidden" name="locid" id="locid" value="<?php echo $rs['locid']; ?>" />
			<input type="hidden" name="requisicao" value="salvarBolsista" />
			<input type="hidden" name="lcbid" value="" />
			
			<table class="listagem" cellpadding="3" cellspacing="1" align="center" width="95%">	
				<tr>
					<td colspan="2" class="subtituloCentro">Incluir Bolsista</td>
				</tr>
				<tr>
					<td class="subtituloDireita" width="40%">Instituição</td>
					<td>
						<?=campo_texto('uniid_auto','S','S','',45,255,'','', 'left', '', '', 'id="uniid_auto"', '', '');?>
						<input type="hidden" name="uniid" id="uniid" value="" />
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">CPF do Responsavel</td>
					<td><?php echo campo_texto('lcbcpftutorsolicitante', 'S', $habilitado, $label, 20, 14, '###.###.###-##', ''); ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nome do Resposável</td>
					<td><?php echo campo_texto('lcbtutorsolicitante', 'S', $habilitado, $label, 45, 255, '', ''); ?></td>
				</tr>
				
				<tr>
					<td class="subtituloDireita">Função do Bolsista</td>
					<td>
					<?php
					$funcao = array(array('descricao'=>'Supervisor', 'codigo'=>'S'), array('descricao'=>'Tutor','codigo'=>'T'));
					$db->monta_combo('lcbfuncao',$funcao,'S','Selecione...',null,null,null,null,'N','lcbfuncao'); 
					?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">CPF do Bolsista</td>
					<td><?php echo campo_texto('lcbcpfsupervisor', 'S', $habilitado, $label, 20, 14, '###.###.###-##', ''); ?></td>
				</tr>
				<tr>
					<td class="subtituloDireita">Nome do Bolsista</td>
					<td><?php echo campo_texto('lcbnomesupervisor', 'S', $habilitado, $label, 45, 255, '', ''); ?></td>
				</tr>
				
				<tr>
					<td class="subtituloDireita">Mês de Referência</td>
					<td>
					<?php 
					$a=0;
					for($x=1;$x<13;$x++){
						$meses_periodo[$a]['codigo'] = str_pad($x,2,'0', STR_PAD_LEFT);
						$meses_periodo[$a]['descricao'] = mes_extenso($x);
						$a++;
					}
					
					$db->monta_combo('lcbmesreferencia',$meses_periodo,'S','Mês',null,null,null,null,'N','lcbmesreferencia');
					
					?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Ano de Referência</td>
					<td>
					<?php 
					$b=0;
					for($y=2012;$y<date('Y')+2;$y++){
						$anos_periodo[$b]['codigo'] = $y;
						$anos_periodo[$b]['descricao'] = $y;
						$b++;
					}
					
					$db->monta_combo('lcbanoreferencia',$anos_periodo,'S','Ano',null,null,null,null,'N','lcbanoreferencia');
					?>
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Bolsa Validada?</td>
					<td>
						<input type="radio" name="lcbbolsavalidada" id="lcbbolsavalidada_t" value="t" />&nbsp;Sim&nbsp;
						<input type="radio" name="lcbbolsavalidada" id="lcbbolsavalidada_f" value="f" />&nbsp;Não
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Relatório Postado?</td>
					<td>
						<input type="radio" name="lcbrelatoriopostado" id="lcbrelatoriopostado_t" value="t" />&nbsp;Sim&nbsp;
						<input type="radio" name="lcbrelatoriopostado" id="lcbrelatoriopostado_f" value="f" />&nbsp;Não
					</td>
				</tr>
				<tr>
					<td class="subtituloDireita">Justificativa do pagamento (arquivo)</td>
					<td><input type="file" name="arquivo" /></td>
				</tr>
			</table>
			
			<br/><br/>
			<p>
				<input type="button" value="Adicionar bolsista" id="btnAddBolsista" />
				<input type="button" value="Novo bolsista" id="btnNewBolsista" style="display:none;"/>
			</p>
					
			<h3>Bolsistas</h3>
			
			<?php 
			
			$sql = "select 
						'
						<img class=\"btnExcluirBolsista\" src=\"../imagens/excluir.gif\" alt=\"Excluir\" title=\"Excluir\" id=\"excluir_' || lcbid || '\" style=\"cursor:pointer\" />
						<img class=\"btnEditarBolsista\" src=\"../imagens/alterar.gif\" alt=\"Editar\" title=\"Editar\" id=\"editar_' || lcbid || '\" style=\"cursor:pointer\" />
						' || case when l.arqid is not null then '<img alt=\"Justificativa\" title=\"Justificativa\" src=\"../imagens/anexo.gif\" class=\"btnAbrirAnexo\" id=\"' || l.arqid || '\" style=\"cursor:pointer;\" />' else '' end  || '' as acao,
						l.locid,
						case when l.lcbinstituicao = '' then u.uninome else l.lcbinstituicao end as instituicao,
						l.lcbtutorsolicitante,
						l.lcbcpftutorsolicitante,
						l.lcbnomesupervisor,
						l.lcbcpfsupervisor,
						case when l.lcbfuncao = 'S' then 'Supervisor' else 'Tutor' end as lcbfuncao,
						case when l.lcbbolsavalidada = 't' then 'Sim' else 'Não' end as lcbbolsavalidada,
						case when l.lcbrelatoriopostado = 't' then 'Sim' else 'Não' end as lcbrelatoriopostado,
						l.lcbmesreferencia || '/' || l.lcbanoreferencia as mes 
					from maismedicos.lotecomplementarbolsista l
					left join maismedicos.universidade u on u.uniid = l.uniid
					where l.lcbstatus = 'A' 
					and l.locid = {$rs['locid']}";
			
			$arCabecalho = array('Ação','Cod. Lote','Instituição','Nome do Responsável','CPF do Responsável','Nome do Bolsista','CPF do Bolsista','Função','Bolsa Validada?','Relatório Postado?', 'Mês de Referência');
			$db->monta_lista($sql, $arCabecalho, 50, 10, '', '', '');
			
			$sql = "select count(*) from maismedicos.autorizacaopagamento where apgstatus = 'A' and lcbid in (
						select lcbid from maismedicos.lotecomplementarbolsista where lcbstatus = 'A' and locid in (
							select max(locid) from maismedicos.lotecomplementar where locstatus = 'A' and locdatafechamento is not null
						)
					) and rmdid is not null";
			
			$fechouUltimoLote = $db->pegaUm($sql);
			?>
			
			<br/><br/>
			<?php if($fechouUltimoLote>0): ?>
				<p><input type="button" value="Fechar lote" id="btnFechaLote" /></p>
			<?php endif; ?>
		
		</form>
		
	</center>
	
<?php else: ?>

	<center>
	
		<p id="txt_abrir_lote"><b>Não existe lote complementar aberto, deseja abrir?</b></p>
		
		<p>
		<input type="radio" name="abrirlote" value="s" />&nbsp;Sim&nbsp;
		<input type="radio" name="abrirlote" value="n" />&nbsp;Não
		</p>
		
		<div id="mes_ano_referencia" style="display:none;">
		
			<?php 
			$a=0;
			for($x=1;$x<13;$x++){
				$meses_periodo[$a]['codigo'] = str_pad($x, 2, '0', STR_PAD_LEFT);
				$meses_periodo[$a]['descricao'] = mes_extenso($x);
				$a++;
			}
			
			$locmespagamento = date('m');
			$db->monta_combo('locmespagamento',$meses_periodo,'S','Mês',null,null,null,null,'N','locmespagamento');
			
			$b=0;
			for($y=date('Y');$y<date('Y')+2;$y++){
				$anos_periodo[$b]['codigo'] = $y;
				$anos_periodo[$b]['descricao'] = $y;
				$b++;
			}
			
			$locanopagamento = date('Y');
			$db->monta_combo('locanopagamento',$anos_periodo,'S','Ano',null,null,null,null,'N','locanopagamento');
			?>
			
			<br/><br/>
			<p><input type="button" value="Criar lote" id="btnCriaLote" /></p>
		
		</div>
		
		<p>&nbsp;</p>
		
		<h3>Lotes fechados</h3>
		<?php 
		$sql = "select 
					'<img src=\"../imagens/excel.gif\" class=\"gerarExcelLote\" id=\"' || l.locid || '\" style=\"cursor:pointer;\" />' 
					||
						case when (select 
									count(*)
								from maismedicos.lotecomplementarbolsista lcb
								join maismedicos.autorizacaopagamento apg on apg.lcbid = lcb.lcbid 
								where lcb.lcbstatus = 'A'
								and apg.rmdid is not null
								and lcb.locid = l.locid)=0 and l.locid = (select max(locid) from maismedicos.lotecomplementar where locstatus = 'A')
						then '&nbsp;<img style=\"cursor:pointer\" class=\"reabrirLote\" src=\"../imagens/refresh2.gif\" title=\"Reabrir o lote\" alt=\"Reabrir o lote\" id=\"' || l.locid || '\"/>' else '' end
					|| 
					'' as acao,
					lpad(l.locid::varchar,6,'0') as locid,
					lpad(l.locmespagamento::varchar, 2, '0') || '/' || l.locanopagamento,
					l.loccpfresponsavel,
					u.usunome,					
					to_char(l.locdataregistro, 'DD/MM/YYYY') as locdataregistro,
					to_char(l.locdatafechamento, 'DD/MM/YYYY') as locdatafechamento,
					(select count(*) from maismedicos.lotecomplementarbolsista s where s.locid = l.locid and s.lcbstatus = 'A' and s.lcbfuncao = 'S') as supervisor,
					(select count(*) from maismedicos.lotecomplementarbolsista t where t.locid = l.locid and t.lcbstatus = 'A' and t.lcbfuncao = 'T') as tutor
				from maismedicos.lotecomplementar l
				left join seguranca.usuario u on u.usucpf = l.loccpfresponsavel
				where l.locstatus = 'A' 
				and l.locdatafechamento is not null
				order by locanopagamento::varchar || lpad(locmespagamento, 2, '0') desc";
// 		ver($sql, d);
		$arCabecalho = array('Ação', 'Cod. Lote', 'Mês Pagamento', 'CPF do Responsável', 'Nome do Responsável', 'Data de Abertura', 'Data de Fechamento', 'Total Supervisores', 'Total Tutores');
		$db->monta_lista($sql, $arCabecalho, 50, 10, 'N', '', 'N');
		?>
	
	</center>
	
<?php endif; ?>
