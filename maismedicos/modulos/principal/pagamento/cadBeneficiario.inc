<?php 


if($_REQUEST['requisicao'] == 'carregaCampi'){

	$sql = "select
	ifcid as codigo,
	ifcnome as descricao
	from maismedicos.ifes_campi
	WHERE  ifeid = {$_REQUEST['ifeid']}";

	echo $db->monta_combo("ifcid", $sql, 'S', 'Selecione...', '', '', '', '200', 'N', 'ifcid', '', '', '', '', '', '', false);
	die;
}

if($_REQUEST['requisicao']=='montaComboMunicipioPorUf'){
	
	header('content-type: text/html; charset=ISO-8859-1');

	if (!$_REQUEST['estuf']) {
		die($this->db->monta_combo("muncod_", array(), 'S', 'Selecione o Estado', '', '', '', '', 'S', 'muncod_', false, null, 'Município'));
	}

	$sql = "select muncod as codigo, mundescricao as descricao
			from territorios.municipio
			where estuf = '" . $_REQUEST['estuf'] . "'
			order by mundescricao asc";
	$muncod = $_REQUEST['muncod'];
	$db->monta_combo("muncod", $sql, 'S', 'Selecione...', '', '', '', '', 'S', 'muncod');	
	die();
}

if($_REQUEST['requisicao'] == 'carregaIfes'){

	$sql = "select
				ifeid as codigo,
				ifenome as descricao
			from maismedicos.ifes
			".($_GET["q"] ? "WHERE ifenome ILIKE '%{$_GET["q"]}%'" : "")."";

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

require_once APPRAIZ . "maismedicos/classes/Pagamento_Beneficiario.class.inc";
$obBen = new Pagamento_Beneficiario();

if($_REQUEST['requisicao']=='VerificaCadastroProfissional'){
	
	$rs = $obBen->recuperaBeneficiarioPorCpf($_REQUEST['cpf']);
	if($rs) die(json_encode(array('nome'=>'existe')));
	
	$rs = $obBen->recuperaTutorSupervisorPorCpf($_REQUEST['cpf']);	
	if($rs) die(json_encode($rs));
	
	$rs = $obBen->recuperaMedicoPorCpf($_REQUEST['cpf']);
	if($rs) die(json_encode($rs));
	
	die;
}

if($_REQUEST['requisicao']=='salvarBeneficiario'){
	if($_POST){
			
		if($_POST['bencpf']){
			
			$_POST['bencpf'] = str_replace(array('.','-'),'',$_POST['bencpf']);
			$_POST['bencep'] = str_replace(array('.','-'),'',$_POST['bencep']);			
			$_POST['benvalidade'] = $_POST['benvalidade']=='S' ? 't' : '';			
			$_POST['bendatanascimento'] = $_POST['bendatanascimento'] ? formata_data_sql($_POST['bendatanascimento']) : '';
			
			foreach($_POST as $i => $v){
				if(empty($v)){
					unset($_POST[$i]);
				}
			}
			
			$obBen->popularDadosObjeto($_POST);
			$obBen->salvar();
			if($obBen->commit()){
				$obBen->clearDados();
				$obBen->sucesso('principal/pagamento/cadBeneficiario', '');
			}
		}
	}
	$obBen->insucesso('Falha ao salvar beneficiário!', '', 'principal/pagamento/cadBeneficiario');
	die;
}

require_once APPRAIZ . "www/includes/webservice/cpf.php";
require_once APPRAIZ . "includes/cabecalho.inc";

echo '<br/>';

monta_titulo('Cadastro de Beneficiário', '');
$db->cria_aba( $abacod_tela, $url, $parametros);


$habilitado = 'S';

if($_GET['benid']){
	$rs = $obBen->recuperaBeneficiario(array('benid'=>$_GET['benid']));
	if($rs){
		extract($rs);
		$bendatanascimento = $bendatanascimento ? formata_data($bendatanascimento) : '';
	}
}
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

<script type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script>
	$(function(){

		var camposObrigatorios  = "[name=uniid_auto],[name=uniid], [name=bencpf], [name=bennome], [name=funid], [name=benemail], [name=bentelefone], ";
		    camposObrigatorios += "[name=estuf], [name=muncod], [name=bencep], [name=benlogradouro], [name=benbairro]";
		    
        $(camposObrigatorios).addClass("required");
        $("[name=benemail]").addClass("email");

        $("#formulario").validate();
/*
        $("[name=uniid_auto]").autocomplete(document.location.href+'&requisicao=carregaUniversidades', {			
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
*/
		$("[name=ifenome]").autocomplete(document.location.href+'&requisicao=carregaIfes', {			
			matchContains: true,
			minChars: 0, 			
			cacheLength:1000,
			width: 440,
			autoFill: false,
			max: 1000    		
		}).result(function(event, data, formatted) {
			var ifeid = data[1];
			$('[name=ifeid]').val(ifeid);
			$.ajax({
				url		: document.location.href,
				type	: 'post',
				data	: '&requisicao=carregaCampi&ifeid='+ifeid,
				success	: function(e){
						$('#td_campi').html(e);
					}
				});
		});

		$('[name=bencpf]').change(function(){

			cpf = $(this).val().replace('.','').replace('.','').replace('-','');

			var atualiza = false;
			$.ajax({
				url		: document.location.href,
				type	: 'post',
				dataType: 'json',
				data	: 'requisicao=VerificaCadastroProfissional&cpf='+cpf,
				success	: function(e){
					if(e.nome=='existe'){
						alert('Beneficiário já cadastrado!');
						$('[name=bencpf]').val('');
						$('[name=bennome]').val('');
					}else
					if(e.nome){
						$('[name=bennome]').val(e.nome);	
						$('[name=funid]').val(e.tipo);
						$('[name=benemail]').val(e.email);
						$('[name=bendatanascimento]').val(e.dtnasc);
						$('[name=bentelefone]').val(e.fone);
						$('[name=bennomemae]').val(e.nomemae);
						$('[name=bencep]').val(e.cep);
						$('[name=benlogradouro]').val(e.logradouro);
						$('[name=bennumero]').val(e.numero);
						$('[name=bencomplemento]').val(e.complemento);
						$('[name=benbairro]').val(e.bairro);
						$('[name=estuf]').val(e.uf);
						if(e.uf) atualizaMunicipios(e.uf, e.ibge);
						$('[name=muncod]').val(e.ibge);
						$('[name=bncid]').val(e.banco);
						$('[name=benagencia]').val(e.agencia);
						$('[name=benconta]').val(e.conta);
						$('[name=bennumbeneficio]').val(e.beneficio);
						//$('[name=uniid]').val(e.uniid);
						//$('[name=uniid_auto]').val(e.uninome);
						atualiza = true;	
					}else{
						
						if( !validar_cpf( cpf  ) ){
							alert( "CPF inválido!\nFavor informar um cpf válido!" );
							$(this).val('');
							$('[name=bennome]').val('');
							return false;	
						}
						
						var comp  = new dCPF();
						comp.buscarDados( cpf );
						if (comp.dados.no_pessoa_rf != ''){
							console.log(comp);
							$('[name=bennome]').val(comp.dados.no_pessoa_rf);				
							$('[name=bennome]').attr("readonly","readonly");
						}
					}
				}
			});

			

		});

		$('.btnEditar').click(function(){
			document.location.href = 'maismedicos.php?modulo=principal/pagamento/cadBeneficiario&acao=A&benid='+this.id;
		});

		$('[name=estuf]').change(function() {

			estuf = this.value;
			var data = new Array();
				data.push({name : 'requisicao', value : 'montaComboMunicipioPorUf'},
						  {name : 'estuf', value : estuf}
						 );

			$.ajax({
				   type		: "POST",
				   url		: document.location.href,
				   data		: data,
				   async    : false,
				   success	: function(res){
								$('#municipio').html(res);
								$("#muncod").addClass("required");
								$('#municipio').css('visibility','visible');
							  }
				 });

		});

		$('[name=btnLimpar]').click(function(){
			
		});

		$('[name=btnPesquisar]').click(function(){
			
		});
	});

	function atualizaMunicipios(estuf, muncod)
	{

		var data = new Array();
			data.push({name : 'requisicao', value : 'montaComboMunicipioPorUf'},
					  {name : 'estuf', value : estuf},
					  {name : 'muncod', value : muncod}
					 );

		$.ajax({
			   type		: "POST",
			   url		: document.location.href,
			   data		: data,
			   async    : false,
			   success	: function(res){
							$('#municipio').html(res);
							$("#muncod").addClass("required");
							$('#municipio').css('visibility','visible');
						  }
			 });
	}
</script>
<form name="formulario" id="formulario" method="post" action="" enctype="multipart/form-data">

	<input type="hidden" name="benid" id="benid" value="<?php echo $benid; ?>" />
	<input type="hidden" name="requisicao" value="salvarBeneficiario" />
	
	<table class="listagem" cellpadding="3" cellspacing="1" align="center" width="95%">	
		<tr>
			<td colspan="2" class="subtituloCentro">Dados do Pessoais</td>
		</tr>
		<tr>
			<td class="subtituloDireita">CPF</td>
			<td>
				<?=campo_texto('bencpf','S','S','',25,14,'###.###.###-##','', 'left', '', '', 'id="bencpf"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome</td>
			<td>
				<?=campo_texto('bennome','S',$habilitado,'',45,255,'','', 'left', '', '', 'id="bennome"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Função</td>
			<td>
			<?php
			$sql = "select 
						funid as codigo,
						funnome as descricao
					from maismedicos.pagamento_beneficiariofuncao
					order by funnome";
			$db->monta_combo('funid',$sql,$habilitado,'Selecione...',null,null,null,null,'S','funid'); 
			?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita" >Data de Nascimento</td>
			<td>
				<?php
					$tutdatanascimento = $tutdatanascimento ? formata_data($tutdatanascimento) : null; 
					echo campo_data2( 'bendatanascimento', 'S', $habilitado, '', 'N','','' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">E-mail</td>
			<td>
				<?=campo_texto('benemail','S',$habilitado,'',45,255,'','', 'left', '', '', 'id="benemail"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Telefone</td>
			<td>
				<?=campo_texto('bentelefone','S',$habilitado,'',45,255,'','', 'left', '', '', 'id="bentelefone"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Instituição</td>
			<td>
				<?=campo_texto('ifenome','S','S','',65,255,'','', 'left', '', '', 'id="ifenome"', '', '');?>
				<input type="hidden" name="ifeid" id="ifeid" value="" />
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Campus</td>
			<td id="td_campi">
			
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Nome da Mãe</td>
			<td>
				<?=campo_texto('bennomemae','S',$habilitado,'',45,255,'','', 'left', '', '', 'id="bennomemae"', '', '');?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtituloCentro">Endereço</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">CEP</td>
			<td>
				<?=campo_texto('bencep','S',$habilitado,'',15,9,'##.###-##','', 'left', '', '', 'id="bencep"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Logradouro</td>
			<td><?=campo_texto('benlogradouro','S',$habilitado,'',65,255,'','', 'left', '', '', 'id="benlogradouro"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número</td>
			<td><?=campo_texto('bennumero','S',$habilitado,'',15,25,'','', 'left', '', '', 'id="bennumero"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Complemento</td>
			<td colspan="3">
				<?=campo_texto('bencomplemento','S',$habilitado,'',65,255,'','', 'left', '', '', 'id="bencomplemento"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Bairro</td>
			<td>
				<?=campo_texto('benbairro','S',$habilitado,'',35,255,'','', 'left', '', '', 'id="benbairro"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Município<br/></td>
			<td>
				<?php
				$sql = "select	
							e.estuf as codigo,
							e.estdescricao as descricao
						from territorios.estado e
						order by e.estdescricao asc";
				
				$db->monta_combo( "estuf", $sql, $habilitado, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'estuf1',false,$estuf,'Estado');
				?>
				<div id="municipio">
				<?php
				$db->monta_combo( "muncod", array(), $habilitado, 'Selecione...', '', '', '', '', 'S', 'muncod',false,null,'Município');
				?>
				</div>
			</td>
		</tr>
			
		<tr>
			<td colspan="2" class="subtituloCentro">Dados Bancários</td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">Banco:</td>
			<td>
				<?php
				$sql = "select 
							bncid as codigo, 
							bncdsc as descricao 
						from maismedicos.banco   
						where bncstatus = 'A' 
						order by bncdsc";
				
				$db->monta_combo( "bncid", $sql, $habilitado, 'Selecione...', 'filtraTipo', '', '', '', 'S', 'bncid',false,'','Banco');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número da Agência</td>
			<td><?=campo_texto('benagencia','S',$habilitado,'',35,255,'','', 'left', '', '', 'id="benagencia"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtituloDireita">Número da Conta</td>
			<td><?=campo_texto('benconta','S',$habilitado,'',35,255,'','', 'left', '', '', 'id="benconta"', '', '');?></td>
		</tr>
		
		<tr>
			<td class="subtituloDireita">Número do benefício:</td>
			<td>
				<?=campo_texto('bennumbeneficio','S',$habilitado,'',35,255,'','', 'left', '', '', 'id="bennumbeneficio"', '', '');?>
			</td>
		</tr>
		<tr>
			<td class="subtituloEsquerda" colspan="2">
				<input type="submit" value="Salvar" />
				<input type="button" value="Limpar" class="btnLimpar"/>
				<input type="button" value="Pesquisar" class="btnPesquisar"/>
			</td>
		</tr>
		
	</table>
	
</form>
<center><h3>Lista de Beneficiários</h3></center>
<?php 
$sql = "select 
			'<img src=\"../imagens/edit_on.gif\" class=\"btnEditar\" id=\"' || benid || '\" style=\"cursor:pointer\"/>' as acao,
			ben.bencpf,
			ben.bennome,
			ben.benemail,
			funnome as funcao,
			to_char(ben.bendatanascimento, 'DD/MM/YYYY') as bendatanascimento,
			bennumbeneficio,
			ife.ifenome,
			ifc.ifcnome,
			mun.mundescricao || '-' || mun.estuf as municipio
		from maismedicos.pagamento_beneficiario ben
		left join maismedicos.pagamento_beneficiariofuncao fun on fun.funid = ben.funid
		left join maismedicos.universidade uni on uni.uniid = ben.uniid
		left join maismedicos.ifes ife on ife.ifeid = ben.ifeid
		left join maismedicos.ifes_campi ifc on ifc.ifcid = ben.ifcid
		left join territorios.municipio mun on mun.muncod = ben.muncod
		where benstatus = 'A'
		order by bennome";

$arCabecalho = array('Ação', 'CPF', 'Nome', 'E-mail', 'Função', 'Data Nascimento', 'Número do Benefício', 'Instituição', 'Campus', 'Município');
$db->monta_lista($sql, $arCabecalho, 100000, 10, '', '', '','');
?>

<?php if($benid): ?>
<script>
$(function(){
	atualizaMunicipios('<?php echo $estuf; ?>', '<?php echo $muncod?>');
	<?php if( $ifeid ): ?>
		$('[name=ifeid]').val('<?php echo $ifeid; ?>');
		$('[name=ifenome]').val('<?php echo $ifenome; ?>');
		$.ajax({
			url		: document.location.href,
			type	: 'post',
			data	: '&requisicao=carregaCampi&ifeid='+<?php echo $ifeid; ?>,
			success	: function(e){
					$('#td_campi').html(e);
					$('[name=ifcid]').val('<?php echo $ifcid; ?>');
				}
			});
	<?php endif; ?>
});
</script>
<?php endif; ?>