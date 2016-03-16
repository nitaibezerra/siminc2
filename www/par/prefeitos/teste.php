<?php
require_once("dompdf/dompdf_config.inc.php");

$html =
  '<html><body><table border="0" align="left" width="100%" cellspacing="0" cellpadding="0" style="font-family: Calibri">
		<thead>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td><img src="imagem/cabecalho.jpg" width="710px" border="0" ></img></td>
					</tr>
					<tr style="color: white; background-color: #FF8C00; text-align: center; font-size: 16px">
						<td>Município: '.$arrDadosMunicipio['nome'].' - '.$arrDadosMunicipio['uf'].'</td>
					</tr>
					<tr style="color: white; background-color: #FFA500; text-align: center; font-family: arial; font-weight: bold">
						<td>Principais ações do Ministério da Educação</td>
					</tr>
				</table>
			</td>
		</tr>
		</thead>
		<tbody>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td align="justify" style="font-family: Calibri"><b>Sistemas e habilitação.</b><br/>
							A situação cadastral da prefeitura nos sistemas corporativos do FNDE, bem como a habilitação do município junto ao órgão são imprescindíveis para acessar os 
							recursos dos diferentes programas do Ministério da Educação. A habilitação objetiva consolidar os documentos legais para efetivação das transferências de 
							recursos pelo FNDE. Já os sistemas informatizados são a porta de entrada para cadastramento de projetos, planejamento das ações educacionais, consultas, 
							monitoramento de informações  entre outros.
						</td>
					</tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="15%">Sistema</th>
							<th width="15%">Situação</th>
							<th width="70%">O que fazer</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Habilitação FNDE / SAPE</td>
							<td>Habilitado, não habilitado. Dado dinâmico.</td>
							<td>Acessar o site do FNDE e consultar a Resolução FNDE nº 10 de 31 de maio de 2012 que prevê os documentos necessários para cadastro.
								<a href="#">http://www.fnde.gov.br/fnde/legislacao/</a></td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Sistema de Gerenciamento de Adesão a Registro de Preços  - SIGARPWEB</td>
							<td>(Informação dinâmica  - fonte SIGARP), Senha ativa, Senha expirada, Senha inexistente</td>
							<td>O sistema permite ao município o acesso  à produtos escolares  padronizados e de qualidade, pela adesão aos registros de preços nacionais, 
								com contratação  de  empresas licitadas pelo FNDE. Para acessar entre  pelos módulos “Produtos -  Adesão on line” ou  “Sistemas” em 
								<a href="#">http://www.fnde.gov.br/portaldecompras/</a></td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
				<tr><td>
					<table border="1" style="border-bottom-style: solid; border-color: black;" align="center" width="100%" cellspacing="0" cellpadding="5">
						<tr style="font-family: Calibri">
							<th width="30%">Sistema</th>
							<th width="30%">O que é:</th>
							<th width="60%">Como acessar:</th>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Simec - PAR</td>
							<td>No Módulo PAR  do Simec o município elabora o seu Plano de Ações Articuladas. Na edição atual, o PAR apresenta as ações e subações  
								para o período de 2011 a 2014.</td>
							<td>Acesse <a href="#">http://simec.mec.gov.br</a><br/>Na tela inicial do Simec, solicitar cadastro, preencher os dados cadastrais e enviar a solicitação. 
								A senha de acesso é enviada para o e-mail informado no cadastro, desde que o endereço eletrônico esteja correto e pertença à pessoa 
								cadastrada - prefeito(a) ou dirigente municipal de educação.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>SIGPC - Contas Online</td>
							<td>Sistema de Gestão de Prestação de Contas, regulamentado pela Resolução CD/FNDE nº 2/2012.</td>
							<td>Acesse    https://www.fnde.gov.br/sigpc   Na tela inicial do Simec, solicitar cadastro, preencher os dados cadastrais e enviar a solicitação. 
								A senha de acesso é enviada para o e-mail informado no cadastro, desde que o endereço eletrônico esteja correto</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>SIOPE</td>
							<td>Sistema de Informações sobre Orçamentos Públicos em Educação.</td>
							<td>Acesse http://www.fnde.gov.br/fnde-sistemas/sistema-siope-apresentacao  nesse endereço o Senhor irá obter todas as informações que o SIOPE 
								disponibiliza para o Público e Órgãos de Controle.</td>
						</tr>
						<tr style="font-family: Calibri; font-size: 12px">
							<td>Portal FNDE</td>
							<td>Sítio de Internet com informações atualizadas sobre ações e programas executados pelo FNDE. Disponibiliza acesso a sistemas, legislação e 
								listagem dos responsáveis na autarquia.</td>
							<td>Acesse http://fnde.gov.br</td>
						</tr>
					</table>
				</td></tr>
				</table>
			</td>
		</tr>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr style="font-family: Calibri; font-weight: bold; font-size: 18px" align="justify">
						<td>Prestação de Contas:</td>
					</tr>
					<tr>
						<td>A prestação de contas tem  a finalidade de comprovar a boa e regular aplicação dos recursos repassados, bem como o cumprimento do objeto 
							e objetivos do programa e/ou projeto. A partir do exercício de 2011, a prestação de contas é por meio eletrônico, utilizando o – “Contas Online” 
							- Sistema de Gestão de Prestação de Contas (SiGPC). O responsável pela entidade é identificado de acordo com o cadastro feito na base corporativa
							do FNDE. Após a atualização, basta solicitar o reenvio da senha para primeiro acesso, o que pode ser feito junto ao Atendimento Institucional, pelo 
							0800 616161, pelo “Fale Conosco” disponível no sítio do FNDE ou acessar o endereço www.fnde.gov.br/sigpc e informar seu CPF e, deixando em branco 
							o campo senha, clicar em “Entrar”, pois esse procedimento automaticamente fará o envio da mensagem com as orientações de acesso ao e-mail registrado 
							no FNDE. Vale esclarecer que o cadastro inicial e o envio da prestação de contas deverá ser realizado pelo gestor (Prefeito).”

						</td>
					</tr>
					<tr>
						<td><table align="center" width="100%" cellspacing="0" cellpadding="3">
								<tr style="background-color: #FF8C00;">
									<td></td>
								</tr>
							</table>
						</td>
					</tr>
				</table>
			</td>
		</tr>
		</tbody>
		<tfoot>
		<tr>
			<td>
				<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td width="10%"><img src="imagem/brasil.png" height="30px;" alt="" ></img></td>
						<td width="10%" align="left"><img src="imagem/fnde.jpg" height="30px;" alt="" ></img></td>
						<td align="right">'.date("j/n/Y H:i:s").'</td>
					</tr>
				</table>
			</td>
		</tr>
		</tfoot>
	</table></body></html>';

$dompdf = new DOMPDF();
$dompdf->load_html($html);
//$dompdf->set_paper('A4');
$dompdf->render();

$dompdf->stream("sample.pdf");




// create some HTML content
//$html = '<h1>Example of HTML text flow</h1>Sed ut perspiciatis unde omnis iste natus error sit voluptatem accusantium doloremque laudantium, totam rem aperiam, eaque ipsa quae ab illo inventore veritatis et quasi architecto beatae vitae dicta sunt explicabo. Nemo enim ipsam voluptatem quia voluptas sit aspernatur aut odit aut fugit, sed quia consequuntur magni dolores eos qui ratione voluptatem sequi nesciunt. Neque porro quisquam est, qui dolorem ipsum quia dolor sit amet, consectetur, adipisci velit, sed quia non numquam eius modi tempora incidunt ut labore et dolore magnam aliquam quaerat voluptatem. <em>Ut enim ad minima veniam, quis nostrum exercitationem ullam corporis suscipit laboriosam, nisi ut aliquid ex ea commodi consequatur?</em> <em>Quis autem vel eum iure reprehenderit qui in ea voluptate velit esse quam nihil molestiae consequatur, vel illum qui dolorem eum fugiat quo voluptas nulla pariatur?</em><br /><br /><b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i> -&gt; &nbsp;&nbsp; <b>A</b> + <b>B</b> = <b>C</b> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>B</i> = <i>A</i> &nbsp;&nbsp; -&gt; &nbsp;&nbsp; <i>C</i> - <i>A</i> = <i>B</i><br /><br /><b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u> <b>Bold</b><i>Italic</i><u>Underlined</u>';

$html = '<table border="0" align="center" width="100%" cellspacing="4" cellpadding="5">
					<tr>
						<td width="10%"><img src="imagem/brasil.png" height="30px;" alt="" ></img></td>
						<td width="10%" align="left"><img src="imagem/fnde.jpg" height="30px;" alt="" ></img></td>
						<td align="right">'.date("j/n/Y H:i:s").'</td>
					</tr>
				</table>';
