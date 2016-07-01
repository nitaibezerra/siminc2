<?php
/**
 * Sistema Integrado de Planejamento, Or&ccedil;amento e Finan&ccedil;as do Ministério da Educa&ccedil;&atilde;o
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Seguran&ccedil;a
 * Finalidade: Tela de apresenta&ccedil;&atilde;o. Permite que o usu&aacute;rio entre no sistema.
 * Data de cria&ccedil;&atilde;o: 24/06/2005
 * Última modifica&ccedil;&atilde;o: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */

// carrega as bibliotecas internas do sistema
require_once 'config.inc';
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
require_once APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conex&atilde;o com o servidor de banco de dados
$db = new cls_banco();

//faz download do arquivo informes
if($_REQUEST['download']){
	$arqid = $_REQUEST['download'];
    DownloadArquivoInfo($arqid);
}

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>alert(\'CPF inv&aacute;lido!\');history.go(-1);</script>');
}

// executa a rotina de autentica&ccedil;&atilde;o quando o formul&aacute;rio for submetido
if ( $_POST['usucpf'] ) {
    if(AUTHSSD) {
        include_once APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include_once APPRAIZ . "includes/autenticar.inc";
    }
}

if ( $_REQUEST['expirou'] ) {
    $_SESSION['MSG_AVISO'][] = "Sua conex&atilde;o expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}

function DownloadArquivoInfo($arqid){
	global $db;
	
	$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
	$arquivo = $db->carregar($sql);
	$caminho = APPRAIZ . 'arquivos/informes/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
	
	if ( !is_file( $caminho ) ) {
		die('<script>alert("Arquivo n&atilde;o encontrado.");</script>');
	}
	
	$filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
	header( 'Content-type: '. $arquivo[0]['arqtipo'] );
	header( 'Content-Disposition: attachment; filename='.$filename);
	readfile( $caminho );
	exit();
}
?>
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

	<!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <link href="library/bootstrap-3.0.0/css/portfolio.css" rel="stylesheet">
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">
	
    <!-- Custom CSS -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">
	<link href="library/simec/css/custom.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="library/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">
    <link href="http://fonts.googleapis.com/css?family=Source+Sans+Pro:300,400,700,300italic,400italic,700italic" rel="stylesheet" type="text/css">

    <!-- HTML5 shim, for IE6-8 support of HTML5 elements -->
    <!--[if lt IE 9]>
    <script src="estrutura/js/html5shiv.js"></script>
    <![endif]-->
    <!--[if IE]>
    <link href="estrutura/temas/default/css/styleie.css" rel="stylesheet">
    <![endif]-->
	
	<!-- Boostrap Scripts -->
    <script src="library/jquery/jquery-1.10.2.js"></script>
    <script src="library/jquery/jquery.maskedinput.js"></script>
    <script src="library/bootstrap-3.0.0/js/bootstrap.min.js"></script>
    <script src="library/chosen-1.0.0/chosen.jquery.min.js"></script>
    <script src="library/bootstrap-switch/js/bootstrap-switch.min.js"></script>
    
	<!-- Custom Scripts -->
    <script type="text/javascript" src="../includes/funcoes.js"></script>
    
    <!-- FancyBox -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/jquery.fancybox.js?v=2.1.5"></script>
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/jquery.fancybox.css?v=2.1.5" media="screen" />
    <script type="text/javascript" src="library/fancybox-2.1.5/lib/jquery.mousewheel-3.0.6.pack.js"></script>

    <!-- Add Button helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.css?v=1.0.5" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-buttons.js?v=1.0.5"></script>

    <!-- Add Thumbnail helper (this is optional) -->
    <link rel="stylesheet" type="text/css" href="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.css?v=1.0.7" />
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-thumbs.js?v=1.0.7"></script>

    <!-- Add Media helper (this is optional) -->
    <script type="text/javascript" src="library/fancybox-2.1.5/source/helpers/jquery.fancybox-media.js?v=1.0.6"></script>
    <style>
    	.panel-heading {
		  background: #F1F1F1;
		  border-radius: 0;
		  border-bottom: 1px solid #DADADA;
		  padding: 18px;
		  position: relative;
		}
		.no-radius {
			border-radius: 0 !important;
		}
    	.th2 {
    		font-size: 12px;
    		font-weight: bold;
    	}
    	.th3 {
    		font-size: 10px;
    		font-weight: bold;
    		font-style: italic;
    	}
    	.center {
    		text-align: center;
    	}
    </style>
</head>

<body class="page-table" data-spy="scroll" data-target="#scrollspy">
	<div class="navbar navbar-<?php echo ( strripos($theme, '-') ) ? 'inverse' : 'default'; ?> navbar-fixed-top">
		<div id="barra-identidade">
			<div id="barra-brasil">
				<div id="wrapper-barra-brasil">
					<div class="brasil-flag"><a class="link-barra" href="http://brasil.gov.br">Brasil</a></div>
					<span class="acesso-info"><a class="link-barra" href="http://brasil.gov.br/barra#acesso-informacao">Acesso à informa&ccedil;&atilde;o</a></span>
					<nav><a id="menu-icon" href="#"></a>
						<ul class="list"><a class="link-barra" href="http://brasil.gov.br/barra#participe">
							<li class="list-item first last-item">Participe</li>
						</a><a class="link-barra" href="http://www.servicos.gov.br/?pk_campaign=barrabrasil">
							<li class="list-item last-item">Servi&ccedil;os</li>
						</a><a class="link-barra" href="http://www.planalto.gov.br/legislacao">
							<li class="list-item last-item">Legisla&ccedil;&atilde;o</li>
						</a><a class="link-barra" href="http://brasil.gov.br/barra#orgaos-atuacao-canais">
							<li class="list-item last last-item">Canais</li>
						</a></ul>
					</nav>
				</div>
			</div>
			<script async="" defer="" type="text/javascript" src="http://barra.brasil.gov.br/barra.js"></script>
		</div>
	
		<header id="top" class="header">
			<div class="row">
				<div class="col-lg-6 col-xs-6 col-sm-6" style="margin-top: 5px;">
					<div class="text-left">
						<img src="estrutura/temas/default/img/logo-simec.png" class="img-responsive" width="200">
					</div>
				</div>
	
				<div class="col-lg-6 col-xs-6 col-sm-6 pull-right" style="margin-top: 5px;">
					<a href="http://www.brasil.gov.br/" class="brasil pull-right"> 
						<img style="margin-right: 10px;" src="http://portal.mec.gov.br/templates/mec2014/images/brasil.png" alt="Brasil - Governo Federal" class="img-responsive"> 
					</a>
				</div>
			</div>
		</header>
		
		<div class="navbar-header">
			<button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
	        	<span class="icon-bar"></span>
	            <span class="icon-bar"></span>
	            <span class="icon-bar"></span>
			</button>
		</div>
		<div class="navbar-collapse collapse nav-menu">
			<div class="row">
				<div class="nav navbar-nav" id="scrollspy">
					<ul class="nav nav-tabs nav-stacked nav-main" data-spy="affix" style="width: 210px">
						<?php for ($i = 0; $i < 20; $i++) : ?>
						<li>
							<a href="#tab<?php echo $i; ?>">
								<i class="fa fa-table" aria-hidden="true"></i>
								<span>Tabela <?php echo $i; ?></span>
							</a>
						</li>
						<?php endfor; ?>
					</ul>
				</div>
			</div>
		</div>
	</div>
	
	<div id="top-shadow" style="display: none; height: 110px; background-color: red; margin-bottom: 30px; box-shadow: #000 0px -1px 18px; position: fixed; top: 0; width: 100%; z-index: 1;"></div>
	
	<div class="inner-wrapper">
		<?php for ($i = 0; $i < 20; $i++) : ?>
		<div id="tab<?php echo $i; ?>" style="position: absolute; height: 20px; margin-top: -100px;"></div>
		<section class="panel no-radius">
			<header class="panel-heading no-radius">
				<h2 class="panel-title no-radius">Tabela <?php echo $i; ?>. Informa&ccedil;&otilde;es sobre o Estado de S&atilde;o Paulo</h2>
			</header>
			<div class="panel-body">
				<div class="table-responsive">
					<table cellspacing="0" cellpadding="1" border="1" summary="Tabula&ccedil;&atilde;o de popula&ccedil;&atilde;o por faixa et&aacute;ria e localiza&ccedil;&atilde;o no município" id="tb2" class="table mb-none table-striped table-hover table-bordered">
						<thead>
						  <tr>
						    <th id="h1" class="th2 center" colspan="10">Popula&ccedil;&atilde;o<span class="upper"> (1)<br>(Localiza&ccedil;&atilde;o / Faixa Et&aacute;ria)</th>
						  </tr>
						  <tr>
						    <th id="h3" class="th2" width="31">Ano</th>
						    <th id="h4" class="th2" width="62">0 a 3 anos</th>
						    <th id="h5" class="th2" width="62">4 a 5 anos</th>
						    <th id="h6" class="th2" width="62">6 a 14 anos</th>
						    <th id="h7" class="th2" width="62">15 a 17 anos</th>
						    <th id="h8" class="th2" width="62">18 a 24 anos</th>
						    <th id="h9" class="th2" width="62">25 a 34 anos</th>
						    <th id="h10" class="th2" width="62">35 anos ou Mais</th>
						    <th id="h11" class="th2" width="62">Total</th>
						  </tr>
						 </thead>
						    <tbody>
                            <tr style="font-size: 8px;">
                        		<th rowspan="3" class="th3">Urbana</th>
                                <td headers="h21 h3" class="th5" width="32">   2000</td>
	                            <td headers="h21 h3" class="th5" width="62">   2.352.789</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.189.479</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.443.382</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">1.986.498</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">4.719.663</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">5.842.010</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">13.052.172</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">34.585.993</td>
	                        </tr>
                            <tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2009</td>
	                            <td headers="h21 h3" class="th5" width="62">   1.856.154</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.009.131</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.341.904</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">1.812.640</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">4.602.362</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">6.540.558</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">17.687.996</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">38.850.745</td>
	                        </tr>
                            <tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2010</td>
	                            <td headers="h21 h3" class="th5" width="62">   2.033.089</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.069.870</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.364.134</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">1.900.557</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">4.744.132</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">7.051.262</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">17.365.594</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">39.528.638</td>
	                        </tr>
                            <tr style="font-size: 8px;">
                        		<th rowspan="3" class="th3">Rural</th>
                                <td headers="h21 h3" class="th5" width="32">   2000</td>
	                            <td headers="h21 h3" class="th5" width="62">   199.481</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">104.959</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">455.084</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">149.658</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">320.014</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">402.543</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">817.672</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">2.449.411</td>
	                        </tr>
                            <tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2009</td>
	                            <td headers="h21 h3" class="th5" width="62">   121.244</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">71.988</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">421.905</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">102.214</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">209.138</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">339.940</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">980.084</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">2.246.513</td>
	                        </tr>
							<tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2010</td>
	                            <td headers="h21 h3" class="th5" width="62">   87.321</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">48.816</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">255.029</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">88.060</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">177.809</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">250.058</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">708.465</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">1.615.558</td>
	                        </tr>
							<tr style="font-size: 8px;">
                        		<th rowspan="3" class="th3">Total</th>
                                <td headers="h21 h3" class="th5" width="32">   2000</td>
	                            <td headers="h21 h3" class="th5" width="62">   2.552.270</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.294.438</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.898.466</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">2.136.156</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">5.039.677</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">6.244.553</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">13.869.844</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">37.035.404</td>
	                        </tr>
							<tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2009</td>
	                            <td headers="h21 h3" class="th5" width="62">   1.977.398</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.081.119</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.763.809</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">1.914.854</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">4.811.500</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">6.880.498</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">18.668.080</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">41.097.258</td>
	                        </tr>
							<tr style="font-size: 8px;">
                                <td headers="h21 h3" class="th5" width="32">   2010</td>
	                            <td headers="h21 h3" class="th5" width="62">   2.120.410</td>
	                            <td headers="h2 h21 h4" class="th5" width="62">1.118.686</td>
	                            <td headers="h2 h21 h5" class="th5" width="62">5.619.163</td>
	                            <td headers="h2 h21 h6" class="th5" width="62">1.988.617</td>
	                            <td headers="h2 h21 h7" class="th5" width="62">4.921.941</td>
	                            <td headers="h2 h21 h8" class="th5" width="62">7.301.320</td>
	                            <td headers="h2 h21 h9" class="th5" width="62">18.074.059</td>
	                            <td headers="h2 h21 h10" class="th5" width="62">41.144.196</td>
	                        </tr>
						    <tr>
								<td id="h11" class="th4 th2"> <acronym title="Produto Interno Bruto">PIB<span class="upper">(2)</span></acronym></td>
								<td class="th4 th2" id="h12" colspan="2"><acronym title="índice de Desenvolvimento Humano">IDH<span class="upper">(3)</span></acronym></td>
								<td class="th4 th2" id="h13" colspan="2"><acronym title="Índice de Desenvolvimento da Infância">IDI<span class="upper">(4)</span></acronym></td>
								<td class="th4 th2" id="h14" colspan="5">Taxa de analfabetismo<span class="upper">(5)</span></td>
						    </tr>
						    <tr style="font-size:8px;">
								<td headers="h11" class="th5" rowspan="2">1.003.015.758</td>
								<td headers="h12" class="th5" rowspan="2" colspan="2">0.820</td>
								<td headers="h13" class="th5" rowspan="2" colspan="2">0.80</td>
								<td headers="h14" id="h142" class="th3" colspan="5">Popula&ccedil;&atilde;o de 15 anos ou mais</td>
						    </tr>
						    <tr>
								<td headers="h14 h142" class="th5" colspan="5" style="font-size:8px;">4.70</td>
						    </tr>
						  </tbody>
						 <tfoot>
						  <tr>
						    <td class="th6 th3" colspan="10">
						    	<p>
						    		<strong>
						    			Fonte: <br>
						    			(1) IBGE - CENSO 2000 E 2010 E PNAD 2009. <br>
						    			(2) IBGE - 2008, a pre&ccedil;os correntes (1 000 R$). <br>
						    			(3) &Iacute;ndice de Desenvolvimento Humano - PNUD - 2000. <br>
						    			(4) &Iacute;ndice de Desenvolvimento da Inf&acirc;ncia - Unicef - 2004. <br>
						    			(5) IBGE - PNAD 2009
						</strong><br>
						        </p>      </td>
						  </tr>
						  </tfoot>						  
						</table>
				</div>
			</div>
		</section>
		<?php endfor; ?>
	</div>
	
    <!-- Custom Theme JavaScript -->
    <script>
    $(function(){
		$('a[href^=#]').on("click",function(){
    	    var t= $(this.hash);
    	    var t=t.length&&t||$('[name='+this.hash.slice(1)+']');
    	    if(t.length){
    	        var tOffset=t.offset().top;
    	        $('html,body').animate({scrollTop:tOffset-20},'slow');
    	        return false;
    	    }
		});
    	$('.navbar-brand').animate({left: 0}, 1500);
        $(window).scroll(function() {
             var coordenada = $(document).scrollTop();
             if (coordenada > 0) {
                 $('#top-shadow').fadeIn('fast');
             } else {
                 $('#top-shadow').fadeOut('fast');
             }
        });
	});
    </script>

</body>

</html>
<?php $db->close(); ?>