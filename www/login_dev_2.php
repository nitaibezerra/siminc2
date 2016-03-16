<?php
/**
 * Sistema Integrado de Planejamento, Orçamento e Finanças do Ministério da Educação
 * Setor responsvel: DTI/SE/MEC
 * Autor: Cristiano Cabral <cristiano.cabral@gmail.com>
 * Módulo: Segurança
 * Finalidade: Tela de apresentação. Permite que o usuário entre no sistema.
 * Data de criação: 24/06/2005
 * Última modificação: 02/09/2013 por Orion Teles <orionteles@gmail.com>
 */

// carrega as bibliotecas internas do sistema
include "config.inc";
require APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/library/simec/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

//faz download do arquivo informes
if($_REQUEST['download']){
//	include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
	$arqid = $_REQUEST['download'];
//	$file = new FilesSimec();
    DownloadArquivoInfo($arqid);
    //echo 'ok';
    //exit;
}

// Valida o CPF, vindo do post
if($_POST['usucpf'] && !validaCPF($_POST['usucpf'])) {
    die('<script>
            alert(\'CPF inválido!\');
            history.go(-1);
         </script>');
}



// executa a rotina de autenticação quando o formulário for submetido
if ( $_POST['usucpf'] ) {

    if(AUTHSSD) {
        include APPRAIZ . "includes/autenticarssd.inc";
    } else {
        include APPRAIZ . "includes/autenticar.inc";
    }
}

if ( $_REQUEST['expirou'] ) {
    $_SESSION['MSG_AVISO'][] = "Sua conexão expirou por tempo de inatividade. Para entrar no sistema efetue login novamente.";
}
?>
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1" />
    <!--        <meta http-equiv="X-UA-Compatible" content="IE=EmulateIE9" /> -->
    <meta content="IE=9" http-equiv="X-UA-Compatible" />

    <title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

    <script type="text/javascript">
        // Testando se é Internet Explorer
//        if (/MSIE (\d+\.\d+);/.test(navigator.userAgent)){
//            var ieversao = new Number(RegExp.$1)
//            // Verificando versão antiga
//            if(ieversao<9){
//                window.location.href = 'login_d.php';
//            }
//        }
    </script>

    <!-- Styles Boostrap -->
    <link href="library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet">
    <!--        <link href="library/bootstrap-3.0.0/css/bootstrap-theme-default.css" rel="stylesheet">-->
    <link href="library/chosen-1.0.0/chosen.css" rel="stylesheet">
    <link href="library/bootstrap-switch/stylesheets/bootstrap-switch.css" rel="stylesheet">

    <!-- Custom Style -->
    <link href="estrutura/temas/default/css/css_reset.css" rel="stylesheet">
    <link href="estrutura/temas/default/css/estilo.css" rel="stylesheet">

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

    <script type="text/javascript">
        $(function(){

            $('.carousel').carousel();

            $('.chosen-select').chosen();

            $('span').tooltip({placement: 'bottom'})

            $('.cpf').mask('999.999.999-99');

            $('#baselogincheck').change(function(){
                if($(this).is(':checked')){
                    $('#baselogin').val('simec_espelho_producao');
                } else {
                    $('#baselogin').val('simec_desenvolvimento');
                }
            });
            
        });
        
        function dinfo(id){
			var url = 'login_dev_2.php?download=' + id;
			var iframe;
	        iframe = document.getElementById("download-container");
	        if (iframe === null)
	        {
	            iframe = document.createElement('iframe');  
	            iframe.id = "download-container";
	            iframe.style.visibility = 'hidden';
	            document.body.appendChild(iframe);
	        }
	        iframe.src = url;
        }
        
        
        
    </script>
    
    <style>
		#barra-brasil .brasil-flag {
			height: 100% !important;
		}
	</style>
	
</head>

<body>
<!-- // Barra do Governo -->
<?php //include_once "barragoverno_2014.php"; ?>
<div id="barra-brasil" style="background:#7F7F7F; height: 20px; padding:0 0 0 10px;display:block;"> 
	<ul id="menu-barra-temp" style="list-style:none;">
		<li style="display:inline; float:left;padding-right:10px; margin-right:10px; border-right:1px solid #EDEDED"><a href="http://brasil.gov.br" style="font-family:sans,sans-serif; text-decoration:none; color:white;">Portal do Governo Brasileiro</a></li> 
		<li><a style="font-family:sans,sans-serif; text-decoration:none; color:white;" href="http://epwg.governoeletronico.gov.br/barra/atualize.html">Atualize sua Barra de Governo</a></li>
	</ul>
</div>
<br />
<div class="container">
<? if ( $_SESSION['MSG_AVISO'] ): ?>
    <div class="alert alert-danger">
        <button type="button" class="close" data-dismiss="alert" aria-hidden="true">&times;</button>
        <?= implode( "<br />", (array) $_SESSION['MSG_AVISO'] ); ?>
    </div>
<? endif;
$_SESSION['MSG_AVISO'] = array(); ?>

<!-- LOGIN -->
<div class="row">
    <div class="col-md-7 col-sm-12">
        <img src="estrutura/temas/default/img/logo-simec.png">
    </div><!-- / .col-md-7 -->

    <div class="col-md-5 col-sm-12 login">

        <form class="form-horizontal" role="form" method="post" action="">
            <input type="hidden" name="baselogin" id="baselogin" value="simec_espelho_producao"/>
            <input type="hidden" name="versao" value="<?php echo $_POST['versao']; ?>"/>
            <div class="col-md-8 col-sm-9">
                <?php /*
                            <div class="form-group">
                                <label for="baselogin" class="hidden-xs col-md-2 col-sm-3 control-label">Base: </label>
                                <div class="col-lg-10 input-group">
                                    <select class="form-control chosen-select" name="baselogin" id="baselogin">
                                        <option  value="simec_desenvolvimento" selected>Desenvolvimento</option>
                                        <option  value="simec_espelho_producao">Espelho da Produção</option>
                                    </select>
                                </div>
                            </div>
                            */ ?>
                <div class="form-group" style="display: block;">
                    <label for="baselogin" class="hidden-xs col-md-2 col-sm-3 control-label">Base: </label>
                    <div class="col-lg-10 input-group">
                        <div class="make-switch" data-on-label="Espelho" data-off-label="Desenv. " data-on="primary" data-off="danger">
                            <input type="checkbox" name="baselogincheck" id="baselogincheck" value="simec_espelho_producao" checked="checked" />
                        </div>
                    </div>
                </div>
                <div class="form-group">
                    <label for="usucpf" class="hidden-xs col-md-2 col-sm-3 control-label">CPF: </label>
                    <div class="col-lg-10 input-group">
                        <span class="input-group-addon"><span class="glyphicon glyphicon-user"></span></span>
                        <input type="text" class="form-control login_input" name="usucpf" id="usucpf" placeholder="Digite o CPF" required="required" onkeyup="this.value=mascaraglobal('###.###.###-##',this.value);">
                    </div>
                </div>
                <div class="form-group">
                    <label for="ususenha" class="hidden-xs col-md-2 col-sm-3 control-label">Senha: </label>
                    <div class="col-lg-10 input-group">
                        <span class="input-group-addon"><span class="glyphicon glyphicon-asterisk"></span></span>
                        <input type="password" class="form-control" name="ususenha" id="ususenha" placeholder="Digite a senha" required="required">
                    </div>
                </div>
            </div>
            <div class="col-md-4 col-sm-3">
                <button type="submit" class="btn btn-xs btn-block btn-success">Entrar</button>
                <a href="recupera_senha.php" class="btn btn-xs btn-block btn-danger">Esqueci a senha</a>
                <a href="cadastrar_usuario.php" class="btn btn-xs btn-block btn-warning">Solicitar acesso</a>
            </div>
        </form>
    </div><!-- / .col-md-5 -->
</div><!-- /LOGIN -->

<div class="hidden-xs">
    <!-- CAROUSEL -->
    <div class="row">
        <div class="col-md-9 col-sm-12" id="informes">
            <div class="box principal">
                <div class="titulo">
                                <span class="text-center">
                                    <div class="texto">Informes</div>
                                </span>
                    <div class="clearfix"></div>
                </div>
                <?php 
                	/*
                	$aInformes = array(
     				'EJA' => '<h2>ATENÇÃO</h2><br>
                              <p>A SECRETARIA DE EDUCAÇÃO CONTINUADA, ALFABETIZAÇÃO, DIVERSIDADE E INCLUSÃO – SECADI informa que a adesão à Resolução 48/2013  (Transferência automática de recursos financeiros aos estados, municípios e Distrito Federal para manutenção de novas turmas de Educação de Jovens e Adultos, foi prorrogada, impreterivelmente, até o dia 31 de janeiro de 2014.</p>',
                               
                    'PROJETO ESPLANADA SUSTENTÁVEL' => '<p>Informamos que o módulo Esplanada Sustentável, utilizado para cadastrar os contratos, acompanhamento das despesas pactuadas e plano de ação , já está em funcionamento.</p>',

                    'PNLD' => '<h2>ATENÇÃO</h2>
                                <p>Aos detentores de direito autoral das obras didáticas inscritas no PNLD/2014.</p>
                                <p>A Secretaria de Educação Básica informa a divulgação do resultado do processo de avaliação realizado no âmbito do PNLD/2014. Os pareceres de todas as obras avaliadas estão disponíveis ao representante da editora cadastrado no SIMAD/FNDE.</p>
                                <p>As informações relativas às fases de interposição de recursos e de correção de falhas pontuais estão disponíveis na portaria do resultado publicado no D.O.U e no Módulo PNLD.</p>',

                    'PRONACAMPO' => '<p>Informo que as ações do Programa Nacional de Educação do Campo ? PRONACAMPO podem ser acessadas por meio do endereço eletrônico: <a href="http://pronacampo.mec.gov.br" target="_blank">http://pronacampo.mec.gov.br</a></p>',

                    'Programação Orçamentária' => '<h2>TERMOS DE COOPERAÇÃO</h2>
                                <p>Informamos que o módulo de descentralização de créditos, utilizado para se cadastrar os TERMOS DE COOPERAÇÃO firmados com as secretarias do Ministério da Educação, já está em funcionamento no módulo de programação orçamentária.</p>
                                <p>O manual de utilização do sistema pode ser visualizado clicando-se no seguinte link: <a href="http://simec.mec.gov.br/Manual_do_Modulo_de_Descentralizacao.pdf">Manual</a>.</p>',

                    'PACTO NACIONAL DE ALFABETIZAÇÃO NA IDADE CERTA' => '<h2>ATENÇÃO</h2>
                                <p>Avisamos aos Estados e Municípios do PACTO que dia <span style="color: #156C94;">15 de Fevereiro</span> encerra-se o prazo para troca de Orientadores de Estudos do PACTO.</p>
                                <p>Assim, a partir dessa data, o Sispacto estará fechado para a execução da ação: <span style="color: #156C94;">Efetuar troca de Orientadores de Estudo</span>.</p>',

                    'Mais Educação' => '<h2>Cadastro de Novas Escolas - Diretores</h2>
                                <p>As escolas interessadas em aderir ao Programa Mais Educação para o ano de 2013 deverão, por meio de seu Diretor (a), solicitar o cadastro no http://simec.mec.gov.br/ no campo ACESSO O SISTEMA - Solicitar Cadastro. </p>
                                <p>O diretor (a) deve selecionar o Módulo ESCOLA, inserir o CPF e continuar. Em seguida, o sistema solicitará os dados pessoais e um perfil, selecionar CADASTRADOR MAIS EDUCAÇÃO. > Enviar solicitação.</p>
                                <p>Após solicitado o cadastro do(a) Diretor(a) um técnico da Secretaria de Educação deverá acessar a página principal do Simec, pois será ele (a) responsável por liberar a senha dos diretores.</p>',

                    'PAR' => '<h2>ATENÇÃO</h2>
                                <p>Sr(a) Usuário do Módulo PAR, </p>
                                <p>Lembramos que o acesso ao PAR municipal pode ser liberado para o(a) prefeito municipal, para o(a) dirigente municipal de educação (DME) e para apenas um(a) técnico(a) indicado(a) pelo(a) DME. No caso dos estados, para o(a) secretário(a) estadual de educação e para os técnicos indicados por ele(a).</p>
                                <p>Os técnicos da secretaria de educação, engenheiros, diretores de escola ou outros usuários de estados e municípios que não foram devidamente autorizados pelo gestor permanecerão bloqueados. </p>
                                <p>No caso de escolas beneficiárias de ações como a construção de quadras escolares, cobertura de quadras existentes ou pelo programa Água na Escola, a apresentação do pleito será feita pelo secretário de educação e sua equipe, não pela escola. </p>',

                    ' Ensino Médio Inovador' => '<h2>A V I S O</h2>
                                <p>O módulo do Programa Ensino Médio Inovador está disponível para ajustes no PRC.</p>
                                <p>Informações sobre recursos financeiros transferidos FNDE, recursos financeiros utilizados no PRC e saldo disponível, acesse a Matriz Orçamentária do PRC de sua escola. </p>',

                    'PDE INTERATIVO' => '<h2>ATENÇÃO</h2>
                                <p>Para acessar o PDE Interativo, faça o seu login no novo endereço: <a target="_blank" href="http://pdeinterativo.mec.gov.br">http://pdeinterativo.mec.gov.br</a></p>',
                ); 
				*/
                
                 // buscando informes
			    $sql = "SELECT ifmtitulo as titulo, ifmtexto as texto, arqid
                        FROM seguranca.informes
                        WHERE ifmstatus='A' AND ifmmodal=false
                        and (
                            (CURRENT_TIMESTAMP >=ifmdatainicio and  ifmdatafim is null) or
                            (CURRENT_TIMESTAMP between ifmdatainicio and ifmdatafim)
                        )
                        ORDER BY ifmid";
			
			    $aInformes = $db->carregar($sql);
				?>

                <div id="carousel-informes" class="carousel slide">
                    <!-- Indicators -->
                    <ol class="carousel-indicators" style="color: black !important;">
                        <?php
                        if($aInformes){
	                        $count = 0;
	                        foreach($aInformes as $v) { ?>
	                            <li data-target="#carousel-informes" data-slide-to="<?php echo $count; ?>" class="<?php echo $count == 0 ? 'active' : ''; ?>"></li>
	                            <?php $count++;
	                        }
						} ?>
                    </ol>

                    <!-- Wrapper for slides -->
                    <div class="carousel-inner">
                        <?php
                        $count = 0;
                        if($aInformes){
	                        foreach($aInformes as $v) { ?>
	                            <div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
	                                <div style="height: 200px !important; background: #fff" class="conteudo">
	                                    <h1><strong><?php echo $v['titulo']; ?></strong></h1>
	                                    <?php 
	                                    	if($v['arqid']){
	                                    		$link = '<a href="javascript:dinfo('.$v['arqid'].')">Clique Aqui</a>';
	                                    		echo str_replace("[LINK]", $link, $v['texto']); 
	                                    	}else{
	                                    		echo $v['texto']; 
	                                    	}
	                                    ?>
	                                </div>
	                            </div>
	                            <?php $count++;
	                        }
						}else{
							?>
							<div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
	                                <div style="height: 200px !important; background: #fff" class="conteudo">
	                                </div>
	                        </div>
							<?
						}	
						?>
						
                    </div>

                    <!-- Controls -->
                    <a class="left carousel-control" href="#carousel-informes" data-slide="prev"><span class="icon-prev"></span></a>
                    <a class="right carousel-control" href="#carousel-informes" data-slide="next"><span class="icon-next"></span></a>
                </div><!-- /#carousel-informes -->
            </div>
        </div><!-- /#informes -->


        <div class="col-md-3 hidden-xs hidden-sm" id="premios">
            <div class="box principal black">
                <div class="titulo">
                                <span class="text-center">
                                    <div class="texto">Prêmios</div>
                                </span>
                    <div class="clearfix"></div>
                </div>

                <?php $aPremios = array(
                    'E-Gov 2013' => 'estrutura/temas/default/img/premios/egov2013.jpg',
                    'E-Gov 2012' => 'estrutura/temas/default/img/premios/premioe-gov2012.png',
                    'E-Gov 2011' => 'estrutura/temas/default/img/premios/premiogovernoti2011.png',
                    'Excelência em Inovação na Gestão Pública - 2010' => 'estrutura/temas/default/img/premios/conip.gif',
                    'E-Gov 2009' => 'estrutura/temas/default/img/premios/premioe-gov.png',
                    'Selo Inovação' => 'estrutura/temas/default/img/premios/selo-inovacao.gif',
                ); ?>

                <div id="carousel-premios" class="carousel slide">
                    <!-- Wrapper for slides -->
                    <div class="carousel-inner" style="height: 200px;">
                        <?php
                        $count = 0;
                        foreach($aPremios as $descricaoPremio => $imgPremio) { ?>
                            <div class="item <?php echo $count == 0 ? 'active' : ''; ?>">
                                <div>
                                    <img class=".img-responsive" src="<?php echo $imgPremio; ?>" alt="<?php echo $descricaoPremio; ?>" title="<?php echo $descricaoPremio; ?>">
                                </div>
                            </div>
                            <?php $count++;
                        } ?>
                    </div>

                    <!-- Controls -->
                    <a class="left carousel-control" href="#carousel-premios" data-slide="prev"><span class="icon-prev"></span></a>
                    <a class="right carousel-control" href="#carousel-premios" data-slide="next"><span class="icon-next"></span></a>
                </div><!-- /#carousel-premios -->
            </div>
        </div><!-- /#premios -->
    </div><!-- /CAROUSEL -->
    <div class="spacer"></div>
</div>

<div class="visible-md visible-lg">

    <?
    // buscando a lista de sistemas
    $sql = "SELECT sisid, sisabrev, sisdsc, sisfinalidade, sispublico, sisrelacionado
                        FROM seguranca.sistema
                        WHERE sisstatus='A' AND sismostra=true
                        ORDER BY sisid";

    $sistemas = $db->carregar($sql);
    ?>

    <div class="row" id="div-sistemas">
        <?php
        $aClasses = array('', 'panel-success', 'panel-warning', 'panel-danger', 'panel-info');
        foreach ($sistemas as $count => $sistema) {

            $class = next($aClasses);
            if('panel-info' == $class){
                reset($aClasses);
            }

            if ($count && !($count%4)) {
                echo '<div class="clearfix"></div>';
            }
            ?>
            <div class="col-md-3 box-sistemas box-login-<?php echo $class; ?>">
                <div class="panel <?php echo $class; ?> bg-<?php echo $class; ?>">
                    <div class="panel-heading">
                        <h3 class="panel-title"><?php echo $sistema['sisabrev']; ?></h3>
                    </div>
                    <div class="panel-body">
                        <img src="estrutura/temas/default/img/modulos/pin-<?php echo $class; ?>.png" style="float: left; margin-right: 10px;" />
                        <span class="span-descricao"><?php echo $sistema['sisdsc']; ?></span>
                        <div class="clearfix"></div>
                        <div style="margin-top: 10px; text-align: center;">
                            <a href="javascript:janela('/geral/fale_conosco.php?sisid=<?php echo $sistema['sisid']; ?>',850,550)"><span data-toggle="tooltip" title="Dúvidas" class="glyphicon glyphicon-info-sign" style="margin-right: 20px;"></span></a>
                            <a href="cadastrar_usuario.php?sisid=<?php echo $sistema['sisid']; ?>"><span data-toggle="tooltip" title="Solicitar Acesso"  class="glyphicon glyphicon-bell" style="margin-right: 20px;"></span></a>
                        </div>
                    </div>
                </div>
            </div>
        <?php } ?>
    </div>
</div>

<div class="hidden-xs hidden-sm">
    <footer>
        <div class="row footer">
            <div class="col-md-12">
                <p>Data do Sistema <?php echo date('d/m/Y')?></p>
            </div>
        </div>
    </footer>
</div>


<div class="modal fade" id="myModal" >
    <div class="modal-dialog">
        <div class="modal-content text-center">

        </div><!-- /.modal-content -->
    </div><!-- /.modal-dialog -->
</div><!-- /.modal -->

</div> <!-- /container -->


<?//mostra show modal informes
// buscando informes
$sql = "SELECT ifmtitulo as titulo, ifmtexto as texto, arqid
		FROM seguranca.informes
        WHERE ifmstatus='A' AND ifmmodal=true
        and (
            (CURRENT_TIMESTAMP >=ifmdatainicio and  ifmdatafim is null) or
            (CURRENT_TIMESTAMP between ifmdatainicio and ifmdatafim)
        )
        ORDER BY ifmid desc";
$aInformesModal = $db->carregar($sql);
if($aInformesModal){
	$i=0;
	foreach($aInformesModal as $v) {
	?>
	<div id="modal-aviso-manutencao-<?=$i?>" class="modal fade modal-informes" >
	    <div class="modal-dialog">
	        <div class="modal-content text-center">
	            <div class="modal-header">
	                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
	                <h4 class="modal-title" style="font-weight: bold;"><?php echo $v['titulo']; ?></h4>
	            </div>
	            <div class="modal-body text-left">
	                <div class="alert alert-danger" style="font-weight: bold">
						<?php 
                             if($v['arqid']){
                             	$link = '<a href="javascript:dinfo('.$v['arqid'].')">Clique Aqui</a>';
                             	echo str_replace("[LINK]", $link, $v['texto']); 
                             }else{
                             	echo $v['texto']; 
                             }
                        ?>
	                </div>
	            </div>
	            <div class="modal-footer">
	                <button type="button" class="btn btn-primary" data-dismiss="modal">Ok</button>
	            </div>
	        </div><!-- /.modal-content -->
	    </div><!-- /.modal-dialog -->
	</div><!-- /.modal -->
	<?}
}?>

<!-- Fim barra governo -->
<script src="//static00.mec.gov.br/barragoverno/barra.js" type="text/javascript"></script>

</body>
</html>


<script language="javascript">
   <?php if($aInformesModal) { ?>
        $('.modal-informes').modal('show');
   <?php } ?>
</script>

<?

function DownloadArquivoInfo($arqid){
		global $db;
		
		$sql ="SELECT * FROM public.arquivo WHERE arqid = ".$arqid;
		$arquivo = $db->carregar($sql);
        $caminho = APPRAIZ . 'arquivos/informes/'. floor($arquivo[0]['arqid']/1000) .'/'.$arquivo[0]['arqid'];
		if ( !is_file( $caminho ) ) {
            die('<script>alert("Arquivo não encontrado.");</script>');
        }
        $filename = str_replace(" ", "_", $arquivo[0]['arqnome'].'.'.$arquivo[0]['arqextensao']);
        header( 'Content-type: '. $arquivo[0]['arqtipo'] );
        header( 'Content-Disposition: attachment; filename='.$filename);
        readfile( $caminho );
        exit();
}

?>