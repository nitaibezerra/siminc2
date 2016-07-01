<!DOCTYPE html> 
<html> 
	<head> 
	<title>SIMEC - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<link rel="stylesheet" href="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.css" />
	<script src="/includes/jquery.mobile-1.0.1/jquery-1.7.1.min.js"></script>	
	<script src="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.js"></script>
	<link type="text/css" href="/includes/jquery.mobile-1.0.1/jquery.mobile.datebox.min.css" rel="stylesheet" />
	<script type="text/javascript" src="/includes/jquery.mobile-1.0.1/jquery.mousewheel.min.js"></script>
	<script type="text/javascript" src="/includes/jquery.mobile-1.0.1/jquery.mobile.datebox.min.js"></script>
	<script>
		$(function() {
			$("#span_total").html($("[name^='hdn_']").length);
			$("#span_estavel").html($("[name='hdn_cor_80BC44']").length);
			$("#span_atencao").html($("[name='hdn_cor_FFC211']").length);
			$("#span_critico").html($("[name='hdn_cor_E95646']").length);
			$("#span_nao_executado").html($("[name='hdn_nao_executado']").length);
			
			$('[name=data]').change(function() {
				$("#span_data").html($('[name=data]').val());
				$("[name='formulario_mobile']").submit();
			});
			
		});
		
		jQuery.mobile.ajaxEnabled = false;
		
		jQuery.extend(jQuery.mobile.datebox.prototype.options.lang, {
	    	'ptbr': {
		        setDateButtonLabel: "Selecionar",
		        setTimeButtonLabel: "Selecionar",
		        setDurationButtonLabel: "Selecionar",
		        calTodayButtonLabel: "Hoje",
		        titleDateDialogLabel: "Selecionar",
		        titleTimeDialogLabel: "Selecionar",
		        daysOfWeek: ['Domingo', 'Segunda', 'Terça', 'Quarta', 'Quinta', 'Sexta', 'Sábado'],
		        daysOfWeekShort: ['Dom', 'Seg', 'ter', 'Qua', 'Qui', 'Sex', 'Sab'],
		        monthsOfYear: ['Janeiro', 'Fevereiro', 'Março', 'Abril', 'Maio', 'Junho', 'Julho', 'Agosto', 'Setembro', 'Outubro', 'Novembro', 'Dezembro'],
		        monthsOfYearShort: ['Jan', 'Fev', 'Mar', 'Abr', 'Mai', 'Jun', 'Jul', 'Ago', 'Set', 'Out', 'Nov', 'Dez'],
		        durationLabel: ['Dias', 'Horas', 'Minutos', 'Segundos'],
		        durationDays: ['Dia', 'Dias'],
		        tooltip: "Abrir Date Picker",
		        nextMonth: "Prómixo Mês",
		        prevMonth: "Mês Anterior",
		        timeFormat: 12,
		        headerFormat: '%A, %-d de %B de %Y.',
		        dateFieldOrder: ['d', 'm', 'y'],
		        timeFieldOrder: ['h', 'i', 'a'],
		        slideFieldOrder: ['d', 'm', 'Y'],
		        dateFormat: '%d/%m/%Y',
		        useArabicIndic: false,
		        isRTL: false,
		        calStartDay: 0,
		        clearButton: 'clear'
		    }
		});
		
		jQuery.extend(jQuery.mobile.datebox.prototype.options, {
		    useLang: 'ptbr'
		});
				
		function filtraEstado(tipo)
		{
			$("[name^='hdn_']").each(function() { 
					$(this).parent().parent().parent().hide();
			});
			if(tipo == "todos"){
				$("[name^='hdn_']").each(function() { 
					$(this).parent().parent().parent().show();
				});
			}
			if(tipo == "estavel"){
				$("[name='hdn_cor_80BC44']").each(function() { 
					$(this).parent().parent().parent().show();
				});
			}
			if(tipo == "atencao"){
				$("[name='hdn_cor_FFC211']").each(function() { 
					$(this).parent().parent().parent().show();
				});
			}
			if(tipo == "critico"){
				$("[name='hdn_cor_E95646']").each(function() { 
					$(this).parent().parent().parent().show();
				});
			}
			if(tipo == "nao_executado"){
				$("[name='hdn_nao_executado']").each(function() { 
					$(this).parent().parent().parent().show();
				});
			}
		}
		
		function exibeMetas(micid,nomeprojeto,projeto,nomemeta){
			window.location.href='estrategico.php?modulo=principal/mobile_estrategico&acao=A&titulo_pagina=Metas&requisicao=exibeMetas&micid=' + micid + '&nomeprojeto=' + nomeprojeto + '&nomemeta=' + nomemeta + '&projeto=' + projeto;
		}
		
	</script>
</head> 
<body>
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-role="page">
	<div data-role="header" data-position="fixed">
		<?php if($_GET['requisicao'] == "listaMetas"): ?>
			<div data-role="controlgroup" data-type="horizontal">
				<a data-transition="flip" href="estrategico.php?modulo=principal/mobile_estrategico&acao=A" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
				<a href="#" data-role="button" data-icon="arrow-r" data-ajax="false"><?php echo $_GET['nomeprojeto'] ?></a>
			</div>
			<a href="#" onclick="$('[name=data]').datebox('open');" data-icon="gear" class="ui-btn-right">Até: <span id="span_data" ><?php echo $_POST['data'] ? $_POST['data'] : date("d/m/Y", mktime(0,0,0,date("m")+1,date("d"),date("Y"))); ?></span></a>
			<div style="right:10px;position:absolute" >
				<input name="data" type="date" data-role="datebox" id="data" value="<?php $_POST['data'] ?>" size="35" data-options='{"mode":"flipbox"}' />
			</div>
			<div data-role="navbar">
				<ul>
					<li><a data-theme="c" class="ui-btn-active" href="#" onclick="javascript:filtraEstado('todos')" >Todas (<span id="span_total">0</span>)</a></li>
					<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('estavel')">Estável (<span id="span_estavel">0</span>)</a></li>
					<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('atencao')">Atenção (<span id="span_atencao">0</span>)</a></li>
					<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('critico')">Crítico (<span id="span_critico">0</span>)</a></li>
					<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('nao_executado')">Não Executado (<span id="span_nao_executado">0</span>)</a></li>
				</ul>
				
			</div>
		<?php elseif($_GET['requisicao'] == "exibeMetas"): ?>
			<div data-role="controlgroup" data-type="horizontal">
				<a data-transition="flip" href="estrategico.php?modulo=principal/mobile_estrategico&acao=A" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
				<a href="estrategico.php?modulo=principal/mobile_estrategico&acao=A&titulo_pagina=Metas&requisicao=listaMetas&nomeprojeto=<?php echo $_GET['nomeprojeto'] ?>&projeto=<?php echo $_GET['projeto'] ?>" data-role="button" data-icon="arrow-r" data-ajax="false"><?php echo $_GET['nomeprojeto'] ?></a>
				<a href="#" data-role="button" data-icon="arrow-r" data-ajax="false"><?php echo $_GET['nomemeta'] ?></a>
			</div>
		<?php else: ?>
			<h1>
				<?php echo $_GET['titulo_pagina'] ? $_GET['titulo_pagina'] : "Selecione o Projeto" ?>
			</h1>
		<? endif; ?>
	</div>
	<div data-role="content">
			<?php $_GET['requisicao'] ? $_GET['requisicao']() : listaProjetosMobile(); ?>
	</div>
	<?php if($_GET['titulo_pagina']): ?>
		<div data-role="footer" data-theme="c" data-position="fixed" >
			<div data-role="controlgroup" data-type="horizontal">
				<a data-transition="pop" href="javascript:history.back(-1)" data-role="button" data-icon="arrow-l" data-iconpos="right">Voltar</a>
		</div>
	<? endif; ?>
</div>
</div>
</form>
</body>
</html>