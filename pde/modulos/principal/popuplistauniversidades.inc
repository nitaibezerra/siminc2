<?php
set_time_limit(0);
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
?>
<!DOCTYPE HTML>
<html>
<head>
	<meta http-equiv="X-UA-Compatible" content="IE=9" />
	<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
	<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>

	<script language="javascript" type="text/javascript" src="/library/jquery/jquery-1.10.2.js"></script>
	<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
	<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>

	<link rel='stylesheet' type='text/css' href='/library/perfect-scrollbar-0.4.5/perfect-scrollbar.css'/>
	<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/jquery.mousewheel.js"></script>
	<script language="javascript" type="text/javascript" src="/library/perfect-scrollbar-0.4.5/perfect-scrollbar.js"></script>

	<link rel='stylesheet' type='text/css' href='/library/jquery_totem/style.css'/>
	<script language="javascript" type="text/javascript" src="/library/jquery_totem/jquery.totemticker.min.js"></script>

	<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
	<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

	<script language="javascript" src="/estrutura/js/funcoes.js"></script>
	<script language="JavaScript" src="../includes/funcoes.js"></script>

	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	<link rel='stylesheet' type='text/css' href='css/cockpit.css'/>
	<script>
		atualizaUsuario();
	</script>

	<style type="text/css">
		#div-ciclos{height: 900px;}
		#div-qtd{height: 550px;}
		.fundo_titulo{background-image:url('../imagens/cockpit/fundo_ed_superior.jpg');background-repeat:repeat-x;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
	</style>
</head>
<body>
	<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="titulo_pagina" >
				<div>
					<img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
				</div>
				<div style="float:right;cursor:pointer;" onclick="window.close();">
					<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
				</div>
			</td>
		</tr>
	</table>
	<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
		<tr>
			<td class="fundo_titulo" style="text-align:center" colspan="3" ><div style="margin:28px" ><?=($_REQUEST['campus'] ? 'Lista de Câmpus' : 'Lista de Universidades') ?></div></td>
		</tr>
		<?php
			$titulo = $_REQUEST['parametro'];
		?>
		<tr>
			<td class="fundo_padrao" colspan="3">
				<div>
					<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
					<div style="float:left" class="titulo_box" ><?=$titulo?><br/> </div>
				</div>
                <?php
                if($_REQUEST['campus']){
                    if($titulo == 'Manuel Deodoro da Fonseca') $where = "BETWEEN '1889-11-01' AND '1891-11-01'";
                    if($titulo == 'Floriano Vieira Peixoto') $where = "BETWEEN '1891-11-02' AND '1894-11-01'";
                    if($titulo == 'Prudente José de Morais e Barros') $where = "BETWEEN '1894-11-02' AND '1898-11-01'";
                    if($titulo == 'Manuel Ferraz de Campos Sales') $where = "BETWEEN '1898-11-02' AND '1902-11-01'";
                    if($titulo == 'Francisco de Paula Rodrigues Alves') $where = "BETWEEN '1902-11-02' AND '1906-11-01'";
                    if($titulo == 'Afonso Augusto Moreira Pena') $where = "BETWEEN '1906-11-02' AND '1909-06-01'";
                    if($titulo == 'Nilo Procópio Peçanha') $where = "BETWEEN '1909-06-02' AND '1910-11-01'";
                    if($titulo == 'Hermes Rodrigues da Fonseca') $where = "BETWEEN '1910-11-02' AND '1914-11-01'";
                    if($titulo == 'Venceslau Brás Pereira Gomes') $where = "BETWEEN '1914-11-02' AND '1918-11-01'";
                    if($titulo == 'Delfim Moreira da Costa Ribeiro') $where = "BETWEEN '1918-11-02' AND '1919-07-01'";
                    if($titulo == 'Epitácio Pessoa') $where = "BETWEEN '1919-07-02' AND '1922-11-01'";
                    if($titulo == 'Artur da Silva Bernardes') $where = "BETWEEN '1922-11-02' AND '1926-11-01'";
                    if($titulo == 'Washington Luís Pereira de Souza') $where = "BETWEEN '1926-11-02' AND '1930-11-01'";
                    if($titulo == 'Getúlio Vargas') $where = "BETWEEN '1930-11-02' AND '1945-10-01'";
                    if($titulo == 'José Linhares') $where = "BETWEEN '1945-10-02' AND '1946-01-01'";
                    if($titulo == 'Gaspar Dutra') $where = "BETWEEN '1946-01-02' AND '1951-01-01'";
                    if($titulo == 'Getúlio Vargas (1951-1954)') $where = "BETWEEN '1951-01-02' AND '1954-08-01'";
                    if($titulo == 'Café Filho') $where = "BETWEEN '1954-08-02' AND '1955-11-01'";
                    if($titulo == 'Nereu de Oliveira Ramos') $where = "BETWEEN '1955-11-02' AND '1956-01-01'";
                    if($titulo == 'Juscelino Kubitschek') $where = "BETWEEN '1956-01-02' AND '1961-01-01'";
                    if($titulo == 'Jânio Quadros') $where = "BETWEEN '1961-01-02' AND '1961-08-01'";
                    if($titulo == 'Pascoal Ranieri Mazzilli') $where = "BETWEEN '1961-08-02' AND '1961-09-01'";
                    if($titulo == 'João Goulart') $where = "BETWEEN '1961-09-02' AND '1964-04-01'";
                    if($titulo == 'Castelo Branco') $where = "BETWEEN '1964-04-02' AND '1967-03-01'";
                    if($titulo == 'Costa e Silva') $where = "BETWEEN '1967-03-02' AND '1969-10-01'";
                    if($titulo == 'Emílio G. Médici') $where = "BETWEEN '1969-10-02' AND '1974-03-01'";
                    if($titulo == 'Ernesto Geisel') $where = "BETWEEN '1974-03-02' AND '1979-03-01'";
                    if($titulo == 'João B. Figueiredo') $where = "BETWEEN '1979-03-02' AND '1985-03-01'";
                    if($titulo == 'José Sarney') $where = "BETWEEN '1985-03-02' AND '1990-03-01'";
                    if($titulo == 'Fernando Collor') $where = "BETWEEN '1990-03-02' AND '1992-12-01'";
                    if($titulo == 'Itamar Franco') $where = "BETWEEN '1992-12-02' AND '1995-01-01'";
                    if($titulo == 'Fernando Henrique Cardoso') $where = "BETWEEN '1995-01-02' AND '2003-01-01'";
                    if($titulo == 'Luiz Inácio Lula da Silva') $where = "BETWEEN '2003-01-02' AND '2011-01-01'";
                    if($titulo == 'Dilma Rousseff') $where = ">= '2011-01-02'";

                    if($titulo == 'Manuel Deodoro da Fonseca') $where = "BETWEEN '1889-11-01' AND '1891-11-01'";
                    if($titulo == 'Floriano Vieira Peixoto') $where = "BETWEEN '1891-11-02' AND '1894-11-01'";
                    if($titulo == 'Prudente Jose de Morais e Barros') $where = "BETWEEN '1894-11-02' AND '1898-11-01'";
                    if($titulo == 'Manuel Ferraz de Campos Sales') $where = "BETWEEN '1898-11-02' AND '1902-11-01'";
                    if($titulo == 'Francisco de Paula Rodrigues Alves') $where = "BETWEEN '1902-11-02' AND '1906-11-01'";
                    if($titulo == 'Afonso Augusto Moreira Pena') $where = "BETWEEN '1906-11-02' AND '1909-06-01'";
                    if($titulo == 'Nilo Procopio Peçanha') $where = "BETWEEN '1909-06-02' AND '1910-11-01'";
                    if($titulo == 'Hermes Rodrigues da Fonseca') $where = "BETWEEN '1910-11-02' AND '1914-11-01'";
                    if($titulo == 'Venceslau Bras Pereira Gomes') $where = "BETWEEN '1914-11-02' AND '1918-11-01'";
                    if($titulo == 'Delfim Moreira da Costa Ribeiro') $where = "BETWEEN '1918-11-02' AND '1919-07-01'";
                    if($titulo == 'Epitacio Pessoa') $where = "BETWEEN '1919-07-02' AND '1922-11-01'";
                    if($titulo == 'Artur da Silva Bernardes') $where = "BETWEEN '1922-11-02' AND '1926-11-01'";
                    if($titulo == 'Washington Luis Pereira de Souza') $where = "BETWEEN '1926-11-02' AND '1930-11-01'";
                    if($titulo == 'Getulio Vargas') $where = "BETWEEN '1930-11-02' AND '1945-10-01'";
                    if($titulo == 'Jose Linhares') $where = "BETWEEN '1945-10-02' AND '1946-01-01'";
                    if($titulo == 'Gaspar Dutra') $where = "BETWEEN '1946-01-02' AND '1951-01-01'";
                    if($titulo == 'Getulio Vargas (1951-1954)') $where = "BETWEEN '1951-01-02' AND '1954-08-01'";
                    if($titulo == 'Cafe Filho') $where = "BETWEEN '1954-08-02' AND '1955-11-01'";
                    if($titulo == 'Nereu de Oliveira Ramos') $where = "BETWEEN '1955-11-02' AND '1956-01-01'";
                    if($titulo == 'Juscelino Kubitschek') $where = "BETWEEN '1956-01-02' AND '1961-01-01'";
                    if($titulo == 'Janio Quadros') $where = "BETWEEN '1961-01-02' AND '1961-08-01'";
                    if($titulo == 'Pascoal Ranieri Mazzilli') $where = "BETWEEN '1961-08-02' AND '1961-09-01'";
                    if($titulo == 'Joao Goulart') $where = "BETWEEN '1961-09-02' AND '1964-04-01'";
                    if($titulo == 'Castelo Branco') $where = "BETWEEN '1964-04-02' AND '1967-03-01'";
                    if($titulo == 'Costa e Silva') $where = "BETWEEN '1967-03-02' AND '1969-10-01'";
                    if($titulo == 'Emilio G. Medici') $where = "BETWEEN '1969-10-02' AND '1974-03-01'";
                    if($titulo == 'Ernesto Geisel') $where = "BETWEEN '1974-03-02' AND '1979-03-01'";
                    if($titulo == 'Joao B. Figueiredo') $where = "BETWEEN '1979-03-02' AND '1985-03-01'";
                    if($titulo == 'Jose Sarney') $where = "BETWEEN '1985-03-02' AND '1990-03-01'";
                    if($titulo == 'Fernando Collor') $where = "BETWEEN '1990-03-02' AND '1992-12-01'";
                    if($titulo == 'Itamar Franco') $where = "BETWEEN '1992-12-02' AND '1995-01-01'";
                    if($titulo == 'Fernando Henrique Cardoso') $where = "BETWEEN '1995-01-02' AND '2003-01-01'";
                    if($titulo == 'Luiz Inacio Lula da Silva') $where = "BETWEEN '2003-01-02' AND '2011-01-01'";
                    if($titulo == 'Dilma Rousseff') $where = ">= '2011-01-02'";

                    if($where) $where = "AND substr(cmp.cmpdataimplantacao,1,4) || '-' || substr(cmp.cmpdataimplantacao,5,6) || '-02' " . $where;
                    $sql = "SELECT
                                entinst.entsig AS universidade,
                                entcampus.entnome AS campus,
                                CASE
                                    WHEN cmp.cmpinstalacao = 'P' THEN 'Provisória'
                                    WHEN cmp.cmpinstalacao = 'D' THEN 'Definitiva'
                                    WHEN cmp.cmpinstalacao = 'S' THEN 'Sem instalações'
                                END AS instalacao,
                                substr(cmp.cmpdataimplantacao,5,6) || '/' || substr(cmp.cmpdataimplantacao,1,4) AS dataatividade
                            FROM entidade.entidade entcampus
                            INNER JOIN entidade.funcaoentidade fn on entcampus.entid = fn.entid and fn.funid = 18 and entcampus.entstatus = 'A' and fn.fuestatus = 'A'
                            INNER JOIN entidade.funentassoc a on a.fueid = fn.fueid
                            INNER JOIN entidade.entidade entinst on entinst.entid = a.entid
                            INNER JOIN entidade.funcaoentidade fn2 on entinst.entid = fn2.entid and fn2.funid = 12 and entinst.entstatus = 'A' and fn2.fuestatus = 'A'
                            INNER JOIN academico.campus cmp on cmp.entid = entcampus.entid
                            INNER JOIN academico.existencia exi on exi.exiid = cmp.exiid
                            WHERE cmp.exiid NOT IN (7)
                            $where
                            ORDER BY dataatividade, universidade, campus, instalacao";
                    $arrDados = $db->carregar( $sql );
                    if($arrDados){
                        ?>
                        <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <th class="center bold" >Universidade</th>
                            <th class="center bold" >Campus</th>
                            <th class="center bold" >Instalação</th>
                            <th class="center bold" >Data</th>
                        </tr>
                        <?php
                        $count=-1;
                        foreach($arrDados as $dado):
                            $count++;
                            ?>
                            <tr class="<?php echo ($count%2) ? 'zebrado' : ''; ?>">
                                <td class="" ><?php echo $dado['universidade'] ?></td>
                                <td class="" ><?php echo $dado['campus'] ?></td>
                                <td class="" ><?php echo $dado['instalacao'] ?></td>
                                <td class="" ><?php echo $dado['dataatividade'] ?></td>
                            </tr>
                        <?php
                        endforeach;
                        ?>
                        <tr>
                            <th class="center bold" colspan="3">Total</th>
                            <th class="numero" ><?=count($arrDados)?></th>
                        </tr>
                        <?php
                    }
                ?>
                <?php }else{?>
                    <table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
                        <tr>
                            <td class="center bold" >Sigla</td>
                            <td class="center bold" >Universidade</td>
                            <td class="center bold" >Ano de Criação</td>
                            <td class="center bold" >Ato de Criação</td>
                        </tr>
                        <?php if(substr(trim($titulo), 0, 8) == 'Epitácio'){ ?>
                            <tr>
                                <td class="" >UFRJ</td>
                                <td class="" >Universidade Federal do Rio de Janeiro</td>
                                <td class="" >1920</td>
                                <td class="" >Decreto nº 14.343 de 07/09/1920</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 7) == 'Getúlio'){ ?>
                            <tr>
                                <td class="" >UFRRJ</td>
                                <td class="" >Universidade Federal Rural do Rio de Janeiro</td>
                                <td class="" >1943</td>
                                <td class="" >Decreto-Lei nº 6.155 de 30/12/1943</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 6) == 'Gaspar'){ ?>
                            <tr>
                                <td class="" >UFBA</td>
                                <td class="" >Universidade Federal da Bahia</td>
                                <td class="" >1946</td>
                                <td class="" >Decreto-Lei nº 9.155 de 08/04/1946</td>
                            </tr>
                            <tr>
                                <td class="" >UFMG</td>
                                <td class="" >Universidade Federal de Minas Gerais</td>
                                <td class="" >1949</td>
                                <td class="" >Lei nº 971 de 16/12/1949</td>
                            </tr>
                            <tr>
                                <td class="" >UFRGS</td>
                                <td class="" >Universidade Federal do Rio Grande do Sul</td>
                                <td class="" >1950</td>
                                <td class="" >Lei nº 1.254 de 04/12/1950</td>
                            </tr>
                            <tr>
                                <td class="" >UFPR</td>
                                <td class="" >Universidade Federal do Paraná</td>
                                <td class="" >1950</td>
                                <td class="" >Lei nº 1.254 de 04/12/1950</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 4) == 'Café'){ ?>
                            <tr>
                                <td class="" >UFC</td>
                                <td class="" >Universidade Federal do Ceara</td>
                                <td class="" >1954</td>
                                <td class="" >Lei nº 2.373 de 16/12/1954</td>
                            </tr>
                            <tr>
                                <td class="" >UFRPE</td>
                                <td class="" >Universidade Federal Rural de Pernambuco</td>
                                <td class="" >1955</td>
                                <td class="" >Lei nº 2.524 de 04/07/1955</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 9) == 'Juscelino'){ ?>
                            <tr>
                                <td class="" >UFPA</td>
                                <td class="" >Universidade Federal do Para</td>
                                <td class="" >1957</td>
                                <td class="" >Lei nº 3.191 de 02/07/1957</td>
                            </tr>
                            <tr>
                                <td class="" >UFPB</td>
                                <td class="" >Universidade Federal da Paraíba</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.835 de 13/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFSCAR</td>
                                <td class="" >Fundação Universidade Federal de São Carlos</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.385 de 13/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFG</td>
                                <td class="" >Universidade Federal de Goiás</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.834-C de 14/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFSM</td>
                                <td class="" >Universidade Federal de Santa Maria</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.834-C de 14/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFF</td>
                                <td class="" >Universidade Federal Fluminense</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.848 de 18/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFRN</td>
                                <td class="" >Universidade Federal do Rio Grande do Norte</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.849 de 18/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFSC</td>
                                <td class="" >Universidade Federal de Santa Catarina</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.849 de 18/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFJF</td>
                                <td class="" >Universidade Federal de Juiz de Fora</td>
                                <td class="" >1960</td>
                                <td class="" >Lei nº 3.858 de 23/12/1960</td>
                            </tr>
                            <tr>
                                <td class="" >UFAL</td>
                                <td class="" >Universidade Federal de Alagoas</td>
                                <td class="" >1961</td>
                                <td class="" >Lei nº 3.867 de 25/01/1961</td>
                            </tr>
                            <tr>
                                <td class="" >UFES</td>
                                <td class="" >Universidade Federal do Espírito Santo</td>
                                <td class="" >1961</td>
                                <td class="" >Lei nº 3.868 de 30/01/1961</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 12) == 'João Goulart'){ ?>
                            <tr>
                                <td class="" >UNB</td>
                                <td class="" >Fundação Universidade de Brasília</td>
                                <td class="" >1962</td>
                                <td class="" >Decreto nº 500 de 15/01/1962</td>
                            </tr>
                            <tr>
                                <td class="" >UFAM</td>
                                <td class="" >Fundação Universidade do Amazonas</td>
                                <td class="" >1962</td>
                                <td class="" >Decreto-Lei nº 4.069-A de 12/06/1962</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 7) == 'Castelo'){ ?>
                            <tr>
                                <td class="" >UFPE</td>
                                <td class="" >Universidade Federal de Pernambuco</td>
                                <td class="" >1965</td>
                                <td class="" >Decreto nº 55.747 de 10/02/1965</td>
                            </tr>
                            <tr>
                                <td class="" >UFMA</td>
                                <td class="" >Fundação Universidade do Maranhão</td>
                                <td class="" >1966</td>
                                <td class="" >Lei nº 5.152 de 21/01/1966</td>
                            </tr>
                            <tr>
                                <td class="" >UFS</td>
                                <td class="" >Fundação Universidade Federal de Sergipe</td>
                                <td class="" >1967</td>
                                <td class="" >Decreto-Lei nº 269 de 28/02/1967</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 5) == 'Costa'){ ?>
                            <tr>
                                <td class="" >UFPI</td>
                                <td class="" >Fundação Universidade Federal do Piauí</td>
                                <td class="" >1968</td>
                                <td class="" >Lei nº 5.558 de 12/11/1968</td>
                            </tr>
                            <tr>
                                <td class="" >UFV</td>
                                <td class="" >Fundação Universidade Federal de Viçosa</td>
                                <td class="" >1969</td>
                                <td class="" >Decreto-Lei nº 570 de 08/05/1969</td>
                            </tr>
                            <tr>
                                <td class="" >UFPEL</td>
                                <td class="" >Fundação Universidade Federal de Pelotas</td>
                                <td class="" >1969</td>
                                <td class="" >Decreto-Lei nº 750 de 08/08/1969</td>
                            </tr>
                            <tr>
                                <td class="" >UFU</td>
                                <td class="" >Fundação Universidade Federal de Uberlândia</td>
                                <td class="" >1969</td>
                                <td class="" >Decreto-Lei nº 762 de 14/08/1969</td>
                            </tr>
                            <tr>
                                <td class="" >FURG</td>
                                <td class="" >Fundação Universidade do Rio Grande - RS</td>
                                <td class="" >1969</td>
                                <td class="" >Decreto-Lei nº 774 de 20/08/1969</td>
                            </tr>
                            <tr>
                                <td class="" >UFOP</td>
                                <td class="" >Fundação Universidade Federal de Ouro Preto</td>
                                <td class="" >1969</td>
                                <td class="" >Decreto-Lei nº 778 de 21/08/1969</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 6) == 'Emílio'){ ?>
                            <tr>
                                <td class="" >UFMT</td>
                                <td class="" >Fundação Universidade Federal de Mato Grosso</td>
                                <td class="" >1970</td>
                                <td class="" >Lei nº 5.647 de 10/12/1970</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 7) == 'Ernesto'){ ?>
                            <tr>
                                <td class="" >UFAC</td>
                                <td class="" >Fundação Universidade Federal do Acre</td>
                                <td class="" >1974</td>
                                <td class="" >Lei nº 6.025 de 05/04/1974</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 6) == 'João B'){ ?>
                            <tr>
                                <td class="" >UNIRIO</td>
                                <td class="" >Fundação Universidade do Rio de Janeiro</td>
                                <td class="" >1979</td>
                                <td class="" >Lei nº 6.655 de 05/06/1979</td>
                            </tr>
                            <tr>
                                <td class="" >UFMS</td>
                                <td class="" >Fundação Universidade Fed.de Mato Gros.do Sul</td>
                                <td class="" >1979</td>
                                <td class="" >Lei nº 6.674 de 05/07/1979</td>
                            </tr>
                            <tr>
                                <td class="" >UNIR</td>
                                <td class="" >Fundação Universidade Federal de Rondônia</td>
                                <td class="" >1982</td>
                                <td class="" >Lei nº 7.011 de 08/07/1982</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 4) == 'José'){ ?>
                            <tr>
                                <td class="" >UFRR</td>
                                <td class="" >Universidade Federal de Roraima</td>
                                <td class="" >1985</td>
                                <td class="" >Lei nº 7.364 de 12/09/1985</td>
                            </tr>
                            <tr>
                                <td class="" >UNIFAP</td>
                                <td class="" >Fundação Universidade Federal do Amapá</td>
                                <td class="" >1986</td>
                                <td class="" >Lei nº 7.530 de 29/08/1986</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 6) == 'Itamar'){ ?>
                            <tr>
                                <td class="" >UFLA</td>
                                <td class="" >Universidade Federal de Lavras</td>
                                <td class="" >1994</td>
                                <td class="" >Lei nº 8.956 de 15/12/1994</td>
                            </tr>
                            <tr>
                                <td class="" >UNIFESP</td>
                                <td class="" >Universidade Federal de São Paulo</td>
                                <td class="" >1994</td>
                                <td class="" >Lei nº 8.957 de 15/12/1994</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 8) == 'Fernando'){ ?>
                            <tr>
                                <td class="" >UFT</td>
                                <td class="" >Fundação Universidade Federal de Tocantins</td>
                                <td class="" >2000</td>
                                <td class="" >Lei nº 10.032 de 23/10/2000</td>
                            </tr>
                            <tr>
                                <td class="" >UFCG</td>
                                <td class="" >Universidade Federal de Campina Grande</td>
                                <td class="" >2002</td>
                                <td class="" >Lei nº 10.419 de 09/04/2002</td>
                            </tr>
                            <tr>
                                <td class="" >UFSJ</td>
                                <td class="" >Universidade Federal de São João del Rei</td>
                                <td class="" >2002</td>
                                <td class="" >Lei nº 10.425 de 19/04/2002</td>
                            </tr>
                            <tr>
                                <td class="" >UNIFEI</td>
                                <td class="" >Universidade Federal de Itajubá - MG</td>
                                <td class="" >2002</td>
                                <td class="" >Lei nº 10.435 de 24/04/2002</td>
                            </tr>
                            <tr>
                                <td class="" >UNIVASF</td>
                                <td class="" >Fund. Universidade Federal Vale São Francisco</td>
                                <td class="" >2002</td>
                                <td class="" >Lei nº 10473 de 27/06/2002</td>
                            </tr>
                            <tr>
                                <td class="" >UFRA</td>
                                <td class="" >Universidade Federal Rural da Amazônia</td>
                                <td class="" >2002</td>
                                <td class="" >Lei nº 10.611 de c23/12/2002</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 4) == 'Luiz'){ ?>
                            <tr>
                                <td class="" >UNIFAL</td>
                                <td class="" >Universidade Federal de Alfenas</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.154 de 29/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFVJM</td>
                                <td class="" >Universidade Fed.Vales Jequitinhonha e Mucuri</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.173 de 06/09/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFTM</td>
                                <td class="" >Universidade Federal do Triangulo Mineiro</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.152 de 29/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFERSA</td>
                                <td class="" >Universidade Federal Rural do semi-árido</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.155 de 29/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UTFPR</td>
                                <td class="" >Universidade Tecnológica Federal do Paraná</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.184 de 07/01/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFABC</td>
                                <td class="" >Fundação Universidade Federal do ABC</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.145 de 26/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFGD</td>
                                <td class="" >Fundação Universidade Fed. da grande dourados</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.153 de 29/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFRB</td>
                                <td class="" >Universidade Federal do Recôncavo da Bahia</td>
                                <td class="" >2005</td>
                                <td class="" >Lei nº 11.151 de 29/07/2005</td>
                            </tr>
                            <tr>
                                <td class="" >UFCSPA</td>
                                <td class="" >Fundação Univ. Fed. de Ciências da Saúde de Porto Alegre</td>
                                <td class="" >2008</td>
                                <td class="" >Lei nº 11.641 de 11/01/2008</td>
                            </tr>
                            <tr>
                                <td class="" >UNIPAMPA</td>
                                <td class="" >Fundação Universidade Federal do Pampa</td>
                                <td class="" >2008</td>
                                <td class="" >Lei nº 11.640 de 11/01/2008</td>
                            </tr>
                            <tr>
                                <td class="" >UFFS</td>
                                <td class="" >Universidade Federal da Fronteira Sul</td>
                                <td class="" >2009</td>
                                <td class="" >Lei nº 11.029 de 15/09/2009</td>
                            </tr>
                            <tr>
                                <td class="" >UFOPA</td>
                                <td class="" >Universidade Federal do Oeste do Pará</td>
                                <td class="" >2009</td>
                                <td class="" >Lei nº 12.085 de 05/11/2009</td>
                            </tr>
                            <tr>
                                <td class="" >UNILA</td>
                                <td class="" >Universidade Federal da Integração Latino-Americana</td>
                                <td class="" >2010</td>
                                <td class="" >Lei nº 12.189 de 12/01/2010</td>
                            </tr>
                            <tr>
                                <td class="" >UNILAB</td>
                                <td class="" >Universidade da Integração Internacional da Lusofonia Afro-Brasileira</td>
                                <td class="" >2010</td>
                                <td class="" >Lei nº 12.289 de 20/07/2010</td>
                            </tr>
                        <?php }elseif(substr(trim($titulo), 0, 5) == 'Dilma'){ ?>
                            <tr>
                                <td class="" >UFCA</td>
                                <td class="" >Universidade Federal do Cariri</td>
                                <td class="" >2013</td>
                                <td class="" >Lei n° 12.826, de 05/06/2013.</td>
                            </tr>
                            <tr>
                                <td class="" >UFOB</td>
                                <td class="" >Universidade Federal do Oeste da Bahia</td>
                                <td class="" >2013</td>
                                <td class="" >Lei n° 12.825, de 05/06/2013.</td>
                            </tr>
                            <tr>
                                <td class="" >UFESBA</td>
                                <td class="" >Universidade Federal do Sul da Bahia</td>
                                <td class="" >2013</td>
                                <td class="" >Lei n° 12.818, de 05/06/2013.</td>
                            </tr>
                            <tr>
                                <td class="" >UNIFESSPA</td>
                                <td class="" >Universidade Federal do Sul Sudeste do Pará</td>
                                <td class="" >2013</td>
                                <td class="" >Lei n° 12.824, de 05/06/2013.</td>
                            </tr>
                        <?php
                        }
                        ?>
                    </table>
                <?php } ?>
			</td>
		</tr>
	</table>
</body>
</html>