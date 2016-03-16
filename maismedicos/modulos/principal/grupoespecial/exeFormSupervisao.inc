<?php 

if($_REQUEST['requisicao'] == 'carregaSupervisores'){

	$sql = "select
				tutcpf as codigo,
				tutcpf || ' - ' || tutnome as descricao
			from maismedicos.tutor
			where tutstatus = 'A'
			and tuttipo = 'S'
			".($_GET["q"] ? " AND tutnome ILIKE '%{$_GET["q"]}%' or tutcpf ILIKE '%{$_GET["q"]}%'" : "")."
			order by tutnome ";

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

if($_REQUEST['requisicao'] == 'carregaMunicipios'){
	
	$sql = "SELECT muncod as codigo, mundescricao as descricao
	FROM territorios.municipio
	WHERE estuf = '{$_REQUEST['uf']}'
	ORDER BY 2 ASC";
	
	$db->monta_combo( "muncod", $sql, 'S', 'Todos municípios', '', '' );
	die;
}

if($_REQUEST['requisicao'] == 'carregaMedicos'){

	$sql = "select
				mdccpf as codigo,
				mdccpf || ' - ' || mdcnome as descricao
			from maismedicos.medico
			where mdcstatus = 'A'
			".($_GET["q"] ? " AND mdcnome ILIKE '%{$_GET["q"]}%' or mdccpf ILIKE '%{$_GET["q"]}%'" : "")."
			order by mdcnome ";

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

if($_REQUEST['requisicao'] == 'carregaDadosMedico'){

	$sql = "select
				mdccpf,
				mdcnome,
				mdcemail,
				mdcnomeinstituicao
			from maismedicos.medico
			where mdcstatus = 'A'
			and mdccpf = '{$_REQUEST['cpf']}'
			order by mdcnome ";
	
	$rs = $db->pegaLinha($sql);
	$arDados['mdccpf'] = $rs['mdccpf'];
	$arDados['mdcnome'] = utf8_encode($rs['mdcnome']);
	$arDados['mdcemail'] = $rs['mdcemail'];
	$arDados['mdcnomeinstituicao'] = utf8_encode($rs['mdcnomeinstituicao']);
	echo json_encode($arDados);
	die;
}

if($_REQUEST['requisicao'] == 'carregaDadosSupervisor'){

	$sql = "select
				tutcpf,
				tutnome
			from maismedicos.tutor
			where tutstatus = 'A'
			and tutcpf = '{$_REQUEST['cpf']}'
			order by tutnome ";
	
	$rs = $db->pegaLinha($sql);
	$arDados['tutcpf'] = $rs['tutcpf'];
	$arDados['tutnome'] = utf8_encode($rs['tutnome']);
	echo json_encode($arDados);
	die;
}

if($_REQUEST['requisicao'] == 'carregaInstituicoes'){

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

require_once APPRAIZ . "www/includes/webservice/cpf.php";
include APPRAIZ.'includes/cabecalho.inc';
echo '</br>';

monta_titulo('Relatório de Supervisão Periódica','Grupo Especial de Supervisão');

?>
<link rel="stylesheet" type="text/css" href="/includes/jquery-validate/css/validate.css" />
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
$j = jQuery.noConflict();

$j(function(){

	$j('#formulario1, #formulario2').hide();
	$j('[name=tutnome],[name=mdcnome],[name=uninome]').css('width','300px');
	$j('.subtitulodireita').css('width','50%');
	$j('textarea').css({'width':'100%','height':'80px'});
	
	$j('[name=supcpf]').blur(function(){
		nuCpf = replaceAll(replaceAll( $j(this).val(),'.',''),'-','');
		if( !validar_cpf( nuCpf  ) ){
			alert( "CPF inválido!\nFavor informar um cpf válido!" );
			$j(this).val('');
			$j('[name=supcpf]').val('');
			return false;	
		}
		var comp  = new dCPF();
		comp.buscarDados( nuCpf );
		if (comp.dados.no_pessoa_rf != ''){
			$j('[name=supnome]').val(comp.dados.no_pessoa_rf);				
			$j('[name=supnome]').attr("readonly","readonly");
		}
	});
	$j("[name=tutnome]").autocomplete(document.location.href+'&requisicao=carregaSupervisores', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var cpf = data[1];
		$j('[name=tutcpf]').val(cpf);
		preencheDadosSupervisor(cpf);
	});
	$j("[name=mdcnome]").autocomplete(document.location.href+'&requisicao=carregaMedicos', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var cpf = data[1];
		$j('[name=mdccpf]').val(cpf);
		preencheDadosMedico(cpf);
	});
	$j("[name=uninome]").autocomplete(document.location.href+'&requisicao=carregaInstituicoes', {			
		matchContains: true,
		minChars: 0, 			
		cacheLength:1000,
		width: 440,
		autoFill: false,
		max: 1000    		
	}).result(function(event, data, formatted) {
		var id = data[1];
		$j('[name=uniid]').val(id);
	});
	$j("[name='estufmunicipio']").change(function(){
		carregaMunicipios(this.value, '#comboMunicipioCampoPratica');
	});

	$j('[name=tipopratica]').change(function(){
		if(this.value == '1'){
			$j('#dadosCadastraisMunicipio').show();
			$j('#dadosCadastraisDsei').hide();
		}else{
			$j('#dadosCadastraisDsei').show();
			$j('#dadosCadastraisMunicipio').hide();
		}
	});


	thisForm = '#formulario0';
	var validator = $j( thisForm ).validate({
		errorClass: "warning",
		onkeyup: false,
		onfocusout: false,
		submitHandler: function() {
			//alert("Submitted, thanks!");
		}
	});

	var arr = [ 'mesref', "anoref", "tutnome",  "mdcnome",  "tiporelatorio" ];
	$j(thisForm).find(':input').each(function(i,e){
		if($j.inArray(e.id, arr)>0){
			$j(e).addClass('required');
		}
	});	

	$j('.btnProximo').click(function(){
		
		thisForm = $j(this).parent().parent().parent().parent().parent();
		idThisForm = $j(thisForm).attr('id');
		if($j( thisForm ).valid()){

			if(idThisForm == 'formulario0'){		
				idProxForm = '#formulario'+$j('[name=tiporelatorio]').val();
			}else{
				idProxForm = '#'+this.id.replace('btn','');
			}
			divCarregando();
			$j.ajax({
				url 	: document.location.href,
				type	: 'post',
				data	: '&'+$j('#'+idThisForm).serialize(),
				success	: function(e){
					$j( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
					$j(thisForm).hide();				
					$j(idProxForm).show();
					divCarregado();
				}
			});
		}
		
	});

	$j('.btnAnterior').click(function(){
		
		thisForm = $j(this).parent().parent().parent().parent().parent();
		idForm = this.id.replace('btn','');
		if(idForm == 'formulario2'){
			tipoForm = $j('[name=tiporelatorio]').val();
			tipoForm = tipoForm==2 ? 0 : 1;
			idProxForm = 'formulario'+tipoForm;
		}else{
			idProxForm = idForm;
		}
		$j( 'html, body' ).animate( { scrollTop: 0 }, 'slow' );
		$j(thisForm).hide();	
		$j('#'+idProxForm).show();	
	});
	
	$j('.btnSalvar').click(function(){
	
		thisForm = $j(this).parent().parent().parent().parent().parent();

		var validator = $j( thisForm ).validate({
			errorClass: "warning",
			onkeyup: false,
			onfocusout: false,
			submitHandler: function() {
				//alert("Submitted, thanks!");
			}
		});
				
		var arr = [ 'mesref', "anoref", "tutnome",  "mdcnome",  "tiporelatorio" ];
		$j(thisForm).find(':input').each(function(i,e){
			if($j.inArray(e.id, arr)){
				$j(e).addClass('required');
			}
		});
		
        $j(thisForm).validate();
	});

	$j('#dadosCadastraisMunicipio, #dadosCadastraisDsei').hide();
	
});

function preencheDadosMedico(cpf)
{
	$j.ajax({
		url		 : document.location.href,
		type	 : 'post',
		dataType : "json",
		data	 : 'requisicao=carregaDadosMedico&cpf='+cpf,
		success	 : function(e){
			if(typeof e.mdcnome == 'undefined'){
				alert('O Médico não foi localizado em nossa base de dados!');
			}else if(e.mdcnome == '' || e.mdcnome == 'null' || e.mdcnome == null){
				alert('Nulo');
			}else{
				$j('[name=medcpf]').val(e.mdccpf);
				$j('[name=mednome]').val(e.mdcnome);
				$j('[name=mednome]').attr("readonly","readonly");
				$j('[name=medemail]').val(e.mdcemail);
				$j('[name=medinstituicao]').val(e.mdcnomeinstituicao);
			}
		}
	});
}

function preencheDadosSupervisor(cpf)
{
	$j.ajax({
		url		 : document.location.href,
		type	 : 'post',
		dataType : "json",
		data	 : 'requisicao=carregaDadosSupervisor&cpf='+cpf,
		success	 : function(e){
			if(typeof e.tutnome == 'undefined'){
				alert('O Supervisor não foi localizado em nossa base de dados!');
			}else if(e.tutnome == '' || e.tutnome == 'null' || e.tutnome == null){
				alert('Nulo');
			}else{
				$j('[name=supcpf]').val(e.tutcpf);
				$j('[name=supnome]').val(e.tutnome);
				$j('[name=supnome]').attr("readonly","readonly");
			}
		}
	});
}

function carregaMunicipios(uf, td)
{
	$j.ajax({
		url		: document.location.href,
		type	: 'post',
		data	: 'requisicao=carregaMunicipios&uf='+uf,
		success	: function (e) {
			$j(td).html(e);
		}
		});
}

//carregaMunicipios('DF', 'comboMunicipioCampoPratica')
</script>
<style>
.txtajuda{
	color: gray;
	font-style: italic;
	font-size: 11px;
}
</style>
<form id="formulario0" name="formulario0" action="" method="post">

	<!-- Primeira parte -->
	
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td class="subtitulodireita">
			MÊS DE REFERÊNCIA DESTE RELATÓRIO<br/>
			<font class="txtajuda">Em qual mês foi realizada esta supervisão</font>
			</td>
			<td>
			<?php
			$a=0;
			for($x=1;$x<13;$x++){
				$meses_periodo[$a]['codigo'] = $x;
				$meses_periodo[$a]['descricao'] = mes_extenso($x);
				$a++;
			}
			
			$db->monta_combo('mesref',$meses_periodo,'S','Selecione...',null,null,null,null,'N','mesref');
			
			$b=0;
			for($y=2013;$y<2018;$y++){
				$anos_periodo[$b]['codigo'] = $y;
				$anos_periodo[$b]['descricao'] = $y;
				$b++;
			}
			
			$db->monta_combo('anoref',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anoref');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">TUTOR(A)</td>
			<td>
				<?=campo_texto('tutnome','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="tutnome"', '', '');?>
				<input type="hidden" name="tutcpf" id="tutcpf" value="" />
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">SUPERVISOR(A)</td>
			<td>
				<?=campo_texto('tutnome','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="tutnome"', '', '');?>
				<input type="hidden" name="tutcpf" id="tutcpf" value="" />
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">MÉDICO(A)</td>
			<td>
				<?=campo_texto('mdcnome','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="mdcnome"', '', '');?>
				<input type="hidden" name="mdccpf" id="mdccpf" value="" />
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				SELECIONE OS DADOS QUE SERÃO INFORMADOS<br/>
				<font class="txtajuda">CASO SEJA O PRIMEIRO LANÇAMENTO DE DADOS DO SUPERVISOR SOBRE O(A) MÉDICO(A), FAVOR PREENCHER, PRIMEIRAMENTE, OS DADOS CADASTRAIS</font>
			</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Cadastrais de Médicos Participantes e Supervisores'),
				array('codigo'=>'2','descricao'=>'Supervisão Periódica')
			);
			$db->monta_combo('tiporelatorio',$dados,'S','Selecione...',null,null,null,null,'N','tiporelatorio');
			?>
			</td>
		</tr>
	</table>
	
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
		<td class="subtitulodireita"></td>
		<td>
		<input type="submit" value="Salvar" class="btnSalvar" />
		<input type="submit" value="Próximo" class="btnProximo" id="btnformulario0" />
		</td>
		</tr>
	</table>
	
</form>

<form id="formulario1" name="formulario1" action="" method="post">
	
	<!-- Segunda parte -->
	
	<table id="dadosCadastrais" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">DADOS CADASTRAIS</font></td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">CADASTRO DO(A) SUPERVISOR(A)</td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>1. DADOS PESSOAIS DO SUPERVISOR</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 CPF</td>
			<td><?=campo_texto('supcpf','N',$habilitado,'',20,255,'###.###.###-##','', 'left', '', '', 'id="supcpf"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.2 Nome</td>
			<td><?=campo_texto('supnome','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="supnome"', '', '');?></td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>2. DADOS PROFISSIONAIS DO(A) SUPERVISOR(A) *</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">2.1 TITULAÇÃO</td>
			<td><?=campo_texto('suptitulacao','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="suptitulacao"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">2.2 CONSELHO PROFISSIONAL</td>
			<td><?=campo_texto('supconcelho','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="supconcelho"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">2.3 INSTITUIÇÃO SUPERVISORA</td>
			<td>
				<?=campo_texto('uninome','N',$habilitado,'',1,255,'','', 'left', '', '', 'id="uninome"', '', '');?>
				<input type="hidden" name="uniid" id="uniid" value="" />
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">CADASTRO DO(A) MÉDICO(A)</td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>3 DADOS PESSOAIS DO(A) MÉDICO(A) SUPERVISIONADO(A)</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">3.1 CPF</td>
			<td><?=campo_texto('medcpf','N',$habilitado,'',20,255,'###.###.###-##','', 'left', '', '', 'id="medcpf"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">3.2 Nome</td>
			<td><?=campo_texto('mednome','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="mednome"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">3.3 NACIONALIDADE</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Cadastrais de Médicos Participantes e Supervisores'),
				array('codigo'=>'2','descricao'=>'Supervisão Periódica')
			);
			$db->monta_combo('mednacionalidade',$dados,'S','Selecione...',null,null,null,null,'N','mednacionalidade');
			?>
			</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">3.4 E-MAIL</td>
			<td><?=campo_texto('medemail','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="medemail"', '', '');?></td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>4 DADOS PROFISSIONAIS DO(A) MÉDICO(A) SUPERVISIONADO(A)</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">4.1 Nome da Instituição de ensino em que se graduou</td>
			<td><?=campo_texto('medinstituicao','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="medinstituicao"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">4.2 Ano de Graduação</td>
			<td>
				<?php 
				$b=0;
				for($y=1970;$y<date('Y');$y++){
					$anos_periodo[$b]['codigo'] = $y;
					$anos_periodo[$b]['descricao'] = $y;
					$b++;
				}
					
				$db->monta_combo('anogradmed',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anogradmed');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">4.2 Conselho Profissional (Órgão de Classe/Número do Registrp/UF)</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'CRM - Brasil'),
				array('codigo'=>'2','descricao'=>'RMS')
			);
			$db->monta_combo('medconpro',$dados,'S','Selecione...',null,null,null,null,'N','mednacionalidade');
			
			echo '&nbsp;&nbsp;&nbsp;Nº&nbsp;'.campo_texto('mednome','N',$habilitado,'',12,255,'','', 'left', '', '', 'id="mednome"', '', '').'&nbsp;&nbsp;&nbsp;UF&nbsp;';
			
			$sql = "select estuf as codigo, estuf as descricao from territorios.estado order by estuf";			
			$db->monta_combo('medufconpro',$sql,'S','Selecione...',null,null,null,null,'N','mednacionalidade');
			?>
			</td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">CADASTRO DO CAMPO DE PRÁTICA</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1. TIPO DE CAMPO DE PRÁTICA</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Município'),
				array('codigo'=>'2','descricao'=>'DSEI')
			);
			$db->monta_combo('tipopratica',$dados,'S','Selecione...',null,null,null,null,'N','tipopratica');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				2. APOIO LOGÍSTICO PARA DESLOCAMENTO<br/>
				<font class="txtajuda">Caso não exista transporte comercial regular para a localidade marque a opção "FORÇAS ARMADAS"</font>
			</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'FIOTEC'),
				array('codigo'=>'2','descricao'=>'Forças Armadas')
			);
			$db->monta_combo('apoiodesloc',$dados,'S','Selecione...',null,null,null,null,'N','apoiodesloc');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">3. TIPO DE TRANSPORTE PARA ACESSO</td>
			<td>
			<input type="checkbox" name="tipotransporte" value="" />&nbsp;TERRESTRE<br/>
			<input type="checkbox" name="tipotransporte" value="" />&nbsp;HIDROVIÁRIO<br/>
			<input type="checkbox" name="tipotransporte" value="" />&nbsp;AÉREO<br/>
			<input type="checkbox" name="tipotransporte" value="" />&nbsp;FERROVIÁRIO<br/>
			<input type="checkbox" name="tipotransporte" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>		
	</table>
	
	<!-- Terceira parte -->
	
	<!-- se for DSEi mostra essa tabela -->
	
	<table id="dadosCadastraisDsei" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">CAMPO DE PRÁTICA - DSEI</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">UF DO DSEI</td>
			<td>
			<?php
			$sql = "select estuf as codigo, estuf as descricao from territorios.estado order by estuf";
			$db->monta_combo('estufdsei',$sql,'S','Selecione...',null,null,null,null,'N','estufdsei');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">NOME DO DSEI</td>
			<td><?=campo_texto('nomedsei','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="nomedsei"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">UF DO POLO BASE</td>
			<td>
			<?php
			$sql = "select estuf as codigo, estuf as descricao from territorios.estado order by estuf";
			$db->monta_combo('estufdseipolo',$sql,'S','Selecione...',null,null,null,null,'N','estufdseipolo');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">NOME DO POLO BASE</td>
			<td><?=campo_texto('nomedseipolo','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="nomedseipolo"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">NOME DA UNIDADE DE SAÚDE</td>
			<td><?=campo_texto('nomeunisaude','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="nomeunisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNES DA UNIDADE DE SAÚDE</td>
			<td><?=campo_texto('cnesunisaude','N',$habilitado,'',15,255,'','', 'left', '', '', 'id="cnesunisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº DE ALDEIAS ATENDIDAS</td>
			<td><?=campo_texto('numaldeias','N',$habilitado,'',8,255,'','', 'left', '', '', 'id="cnesunisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº DE POPULAÇÃO ATENDIDA</td>
			<td><?=campo_texto('numpopula','N',$habilitado,'',8,255,'','', 'left', '', '', 'id="cnesunisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">ETNIAS ATENDIDAS</td>
			<td><?php echo campo_textarea( 'etiniasatendidas', 'N', $habilitado, '', 25, 4, 255, '', '', '', '', '','', array('id'=>'etiniasatendidas'));?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">COMPOSIÇÃO DA EQUIPE MULTIDISCIPLINAR DE ATENÇÃO À SAÚDE INDÍGENA</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
	</table>
	
	<!-- se for Municipio mostra essa tabela -->
	
	<table id="dadosCadastraisMunicipio" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">CAMPO DE PRÁTICA - MUNICÍPIO</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">UF DO MUNICÍPIO</td>
			<td>
			<?php
			$sql = "select estuf as codigo, estuf as descricao from territorios.estado order by estuf";
			$db->monta_combo('estufmunicipio',$sql,'S','Selecione...',null,null,null,null,'N','estufmunicipio');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">NOME DO MUNICÍPIO</td>
			<td id="comboMunicipioCampoPratica">
				<?php
				//campo_texto('nomemunicipio','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="nomemunicipio"', '', '');
				$db->monta_combo( "muncod", array(), 'S', 'Selecione...', '', '' );
				?>
			</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">NOME DA UNIDADE DE SAÚDE</td>
			<td><?=campo_texto('nomemunicipiounisaude','N',$habilitado,'',35,255,'','', 'left', '', '', 'id="nomemunicipiounisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">MODALIDADE DE EQUIPE DE ATENÇÃO BÁSICA</td>
			<td>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;EQUIPE PACS<br/>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;ESTRATÉGIA SAÚDE DA FAMÍLIA<br/>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;EQUIPE RIBEIRINHA<br/>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;EQUIPE FLUVIAL<br/>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;UNIDADE MISTA<br/>
			<input type="checkbox" name="modalidadeequipe" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNES DA UNIDADE DE SAÚDE</td>
			<td><?=campo_texto('cnesunisaude','N',$habilitado,'',15,255,'','', 'left', '', '', 'id="cnesunisaude"', '', '');?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nº DE POPULAÇÃO ATENDIDA</td>
			<td><?=campo_texto('numpopula','N',$habilitado,'',8,255,'','', 'left', '', '', 'id="cnesunisaude"', '', '');?></td>
		</tr>
	</table>
	
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
		<td class="subtitulodireita"></td>
		<td>
		<input type="button" value="Anterior" class="btnAnterior" id="btnformulario0" />
		<input type="button" value="Salvar" class="btnSalvar" id="btnformulario1" />
		<input type="button" value="Próximo" class="btnProximo" id="btnformulario2" />
		</td>
		</tr>
	</table>
	
</form>

<form id="formulario2" name="formulario2" action="" method="post">
	
	<!-- Quarta parte -->
	
	<table id="supervisaoPeriodica" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">SUPERVISÃO PERIÓDICA</font></td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+0">PACTUAÇÃO DA SUPERVISÃO</font></td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>1 DADOS DA PACTUAÇÃO DA SUPERVISÃO PERIÓDICA</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 Neste mês, você conseguiu contato prévio com o(a) médico(a) e a gestão local para pactuar a realização da supervisão?</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Sim'),
				array('codigo'=>'2','descricao'=>'Não')
			);
			$db->monta_combo('contatoprevio',$dados,'S','Selecione...',null,null,null,null,'N','contatoprevio');
			?>
			</td>
		</tr>
	</table>
	
	<!-- se for SIM mostra essa tabela -->
	
	<table id="supervisaoPeriodicaModalidade" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+0">MODALIDADE DE SUPERVISÃO PERIÓDICA</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.2 Modalidade de supervisão periódica:</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Visita In Loco'),
				array('codigo'=>'2','descricao'=>'Acompanhamento Longitudinal')
			);
			$db->monta_combo('modalidadesup',$dados,'S','Selecione...',null,null,null,null,'N','modalidadesup');
			?>
			</td>
		</tr>
		
		<!-- mostrar somente se for longitudinal -->
		
		<tr>
			<td class="subtitulodireita">1.3 Em se tratando de acompanhamento longitudinal, qual modalidade de comunicação foi utilizada?</td>
			<td>
			<input type="checkbox" name="acomplong" value="" />&nbsp;telefone<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;webconferência (por telessaúde, Skype, hangout, outro.)<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;e-mail<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaRealizacao" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">REALIZAÇÃO DA SUPERVISÃO</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 Data de início da Supervisão</td>
			<td><?php echo campo_data2( 'ptadtprevisto_visita', 'N', $habilitado, '', 'N','','' );?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 Data de conclusão da Supervisão</td>
			<td><?php echo campo_data2( 'ptadtprevisto_visita', 'N', $habilitado, '', 'N','','' );?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.2 O(a) médico(a) compareceu ao momento de supervisão</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Sim'),
				array('codigo'=>'2','descricao'=>'Não')
			);
			$db->monta_combo('contatoprevio',$dados,'S','Selecione...',null,null,null,null,'N','contatoprevio');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Com que atores e por que não houve contato prévio para pactuação da supervisão?</td>
			<td>
			<input type="checkbox" name="acomplong" value="" />&nbsp;MÉDICO<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;EQUIPE DE SAÚDE<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;GESTOR LOCAL<br/>
			<input type="checkbox" name="acomplong" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaAbsenteismo" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">ABSENTEÍSMO</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				1. INFORME SITUAÇÕES DE ABSENTEÍSMO:<br/>
				<font class="txtajuda">A gestão/Equipe de Atenção Básica informou que o(a) médico(a)</font>
			</td>
			<td>
				<input type="radio" name="acomplong" value="" />&nbsp;Foi remanejado(a) para outro município<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Foi desligado(a) do Projeto<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Foi afastado(a) das atividades por descanso médico<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Está de atestado por licença maternidade<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Está de atestado por doença<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Não justificou a ausência<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Está supenso(a) das atividades do Projeto<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaPlanejamento" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">PLANEJAMENTO DA SUPERVISÃO</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1. Este mês qual seu objetivo neste momento de supervisão?</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Qualificação da atenção médica (núcleo)'),
				array('codigo'=>'2','descricao'=>'Qualificação da atenção à saúde Multidisciplinar (campo)'),
				array('codigo'=>'2','descricao'=>'Promover espaço de educação permanente para o médico e equipe')
			);
			$db->monta_combo('contatoprevio',$dados,'S','Selecione...',null,null,null,null,'N','contatoprevio');
			?>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaPlanejamentoQualificacao" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+0">QUALIFICAÇÃO DA ATENÇÃO MÉDICA (NÚCLEO)</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 Que questões a supervisão pretende abordar com o médico nesta supervisão?</td>
			<td>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Aperfeiçoamento da Conduta Clínica<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Aperfeiçoamento da Terapêutica<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Realizar interconsulta no unidade de saúde ou em visita domiciliar<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Apoiar a implementação do projeto de intervenção<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Discutir a utilização do telessaúde<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Discutir a utilização do E-SUS<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Discutir Protocolo clínico da Atenção Básica<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Discutir implantação de acolhimento com classificação de risco<br/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				1.2 Caso você vá trabalhar a conduta clínica<br/>
				<font class="txtajuda">Caso você tenha identificado que o(a) médico(a) precisa trabalhar manejo clínico, qual(is) protocolo(s) de atenção básica de quai(s) linhas de cuidado será(ão) trabalhada(s)?</font>
			</td>
			<td>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde da Criança<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde da Mulher<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde do Homem<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde do Idoso<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde Mental<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde da pessoa com deficiência<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde Indígena<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Saúde da população negra<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Imunização<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Planejamento Familiar<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Pré-natal e puericultura<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Dengue e chikungunya<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Hanseníase<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Tuberculose<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Hipertensão<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Diabetes<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Pequenas cirurgias<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Urgência e ermergência<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaMetodologia" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">METODOLOGIA UTILIZADA</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.1 Descreva suas impressões sobre a necessidade de potencializar as ações identificadas no ponto anterior, como você identificou esta necessidade e o que e como pretende trabalhar a equipe, ou evolua observação em caso de continuidade da ação do mês anterior.</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.2 Quais ferramentas metodológicas serão utilizadas neste momento de supervisão?</td>
			<td>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Feedback<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;CAPE (Ciclo de aperfeiçoamento profissional da equipe)<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;CAPP (Ciclo de aperfeiçoamento profissional pessoal)<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Planilha de necessidades<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Análise de vídeo-gravação de consulta ou grupo<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Treinamento de habilidades<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Revisão de casos clínicos e eventos críticos<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Revisão de prontuários<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Construção de protocolo interno (local)<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Adequação e validação de protocolo externo (Estadual, Nacional, Internacional)<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Exposição dialogada<br/>
				<input type="checkbox" name="acomplong" value="" />&nbsp;Consulta Simulada<br/>
				<input type="radio" name="acomplong" value="" />&nbsp;Outro&nbsp;<input type="text" name="tipotransporteoutro" /><br/>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">1.3 Descreva a proposta metodológica a ser utilizada na supervisão</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
	</table>
	
	
	<table id="supervisaoPeriodicaAuxilio" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">CONDIÇÕES DE MORADIA E AUXÍLIO ALIMENTAÇÃO</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">
				A situação de alimentação e moradia permanece inalterada em relação as informações relatadas no mês anterior?<br/>
				<font class="txtajuda">Se esta é a primeira vez que você está preenchendo este formulário, por favor assinale "Não"</font>
			</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Sim'),
				array('codigo'=>'2','descricao'=>'Não')
			);
			$db->monta_combo('contatoprevio',$dados,'S','Selecione...',null,null,null,null,'N','contatoprevio');
			?>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaOfertas" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">CONDIÇÕES DE UTILIZAÇÃO DAS OFERTAS EDUCACIONAIS DO PROJETO</font></td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+0">QUAL A SITUAÇÃO DO MÉDICO PERANTE AS OFERTAS EDUCACIONAIS DO PROJETO</font></td>
		</tr>
		<tr>
			<td colspan="2">
				<center>
				<table width="100%">
					<tr>
						<td></td>
						<td>NÃO HÁ CONECTIVIDADE NO MUNICÍPIO PARA ACESSAR</td>
						<td>HÁ CONCECTIVIDADE NO MUNICÍPIO MAS NUNCA ACESSOU</td>
						<td>HÁ CONECTIVIDADE, ESTÁ CADASTRADO  MAS TEM DÚVIDA NA UTILIZAÇÃO DA OFERTA</td>
						<td>HÁ CONCECTIVIDADE MAS SE RECUSA A UTILIZAR A OFERTA</td>
					</tr>
					<tr>
						<td>ESPECIALIZAÇÃO</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>TELESSAÚDE</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>PORTAL SAÚDE BASEADA EM EVIDÊNCIAS</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>RELATÓRIO DE PRIMEIRAS IMPRESSÕES</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
				</table>
				</center>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaAtencaoBasica" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">PROCESSO DE TRABALHO NA ATENÇÃO BÁSICA</font></td>
		</tr>
		<tr>
			<td colspan="2" align="left"><b>CONDIÇÕES DE ESTRUTURA DA UNIDADE DE SAÚDE</b></td>
		</tr>
		<tr>
			<td class="subtitulodireita">HOUVE ALGUMA MUDANÇA NA ESTRUTURA DA UNIDADE?</td>
			<td>
			<?php 
			$dados = array(
				array('codigo'=>'1','descricao'=>'Sim'),
				array('codigo'=>'2','descricao'=>'Não')
			);
			$db->monta_combo('contatoprevio',$dados,'S','Selecione...',null,null,null,null,'N','contatoprevio');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">DESCREVA AS MUDANÇAS IDENTIFICADAS</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
	</table>
		
		
	<table id="supervisaoPeriodicaAtividadeEduc" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td colspan="2" class="subtitulocentro"><font size="+1">ATIVIDADES EDUCACIONAIS</font></td>
		</tr>
		<tr>
			<td class="subtitulodireita">OBSERVAÇÕES</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
		<tr>
			<td colspan="2" class="subtitulocentro">
				QUAIS TEMAS E MEIOS DE FORMAÇÃO FORAM UTILIZADOS PELO MÉDICO NESTE MÊS?<br/>
				<font class="txtajuda">Averiguar se houve algum processo educativo relacionado às temáticas da Atenção Básica e que meios foram ofertados/utilizados pelo médico para tal.</font>
			</td>
		</tr>
		<tr>
			<td colspan="2">
				<table width="100%">
					<tr>
						<td></td>
						<td>ESPECIALIZAÇÃO</td>
						<td>TELESSAÚDE</td>
						<td>PORTAL SAÚDE BASEADA EM EVIDÊNCIAS</td>
						<td>MÓDULOS EDUCACIONAIS DA UNA-SUS</td>
						<td>PROCESSO DE EDUCAÇÃO PERMANENTE PROMOVIDOS PELA GESTÃO LOCAL</td>
						<td>OUTROS</td>
						<td>NENHUM</td>
					</tr>
					<tr>
						<td>PROCESSO DE TRABALHO</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>PLANEJAMENTO E GESTÃO</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>SAÚDE DA CRIANÇA</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>SAÚDE DA MULHER</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>SAÚDE MENTAL</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>SAÚDE DO ADULTO</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>SAÚDE DO IDOSO</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>URGÊNCIA E EMERGÊNCIA</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>ATENÇÃO DOMICILIAR</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>EDUCAÇÃO EM SAÚDE</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
					<tr>
						<td>OUTROS</td>
						<td><input type="radio" name="opc1" /></td>
						<td><input type="radio" name="opc2" /></td>
						<td><input type="radio" name="opc3" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
						<td><input type="radio" name="opc4" /></td>
					</tr>
				</table>
				</center>
			</td>
		</tr>
	</table>
	
	<table id="supervisaoPeriodicaDificuldades" class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
			<td class="subtitulodireita">DIFICULDADES RELATADAS PELO MÉDICO NESTE MÊS NA REALIZAÇÃO DE SUAS ATIVIDADES</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">AVANÇOS RELATADOS PELO MÉDICO NESTE MÊS NA REALIZAÇÃO DE SUAS ATIVIDADES</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">ORIENTAÇÕES REPASSADAS AO MÉDICO PARTICIPANTE PELA SUPERVISÃO</td>
			<td><?php echo campo_textarea( 'composicaoequipe', 'N', $habilitado, '',25, 4, 255, '', '', '', '', '','', array('id'=>'composicaoequipe'));?></td>
		</tr>
	</table>
	
	<table class="tabela" cellpadding="3" cellspacing="1" align="center" width="95%">
		<tr>
		<td class="subtitulodireita"></td>
		<td>
		<input type="button" value="Anterior" class="btnAnterior" id="btnformulario2" />
		<input type="button" value="Salvar" class="btnSalvar" />
		<input type="button" value="Enviar" class="btnEnviar" />
		</td>
		</tr>
	</table>

</form>