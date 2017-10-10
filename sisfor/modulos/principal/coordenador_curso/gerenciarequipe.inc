<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="/includes/JQuery/jquery-ui-1.8.4.custom/css/jquery-ui.css" rel="stylesheet" type="text/css"/>
<script src="/includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-ui-1.8.4.custom.min.js"></script> 
<script type=text/javascript src=/includes/prototype.js></script>
<script language="javascript" type="text/javascript" src="../includes/webservice/cpf.js" /></script>
<script>

<? if ($consulta) : ?>
        jQuery(document).ready(function() {
            jQuery("[name^='chk[']").css('display', 'none');
            jQuery("[src='../imagens/icon_campus_1.png']").css('display', 'none');
            jQuery("[src='../imagens/refresh.gif']").css('display', 'none');
            jQuery("[src='../imagens/excluir.gif']").css('display', 'none');

            jQuery("#marcartodos").css('display', 'none');

            jQuery("#ativarmarcados").css('display', 'none');
            jQuery("#bloquearmarcados").css('display', 'none');

        });
<? endif; ?>


    

    function enviarMarcado(suscod) {
       
       
        var tchk = jQuery("[name^='chk[']:enabled:checked").length;

        if (tchk == 0) {
            alert('Selecione os Usuários para enviar');
            return false;
        }

        var sit = new Array();
        sit['A'] = 'Ativar';
        sit['B'] = 'Bloquear';
        conf = confirm('Deseja realmente ' + sit[suscod] + ' todos os usuários marcados?');
        if (conf) {
            var input = document.createElement("input");
            input.setAttribute("type", "hidden");
            input.setAttribute("name", "suscod");
            input.setAttribute("value", suscod);
            document.getElementById("formulario").appendChild(input);


            var input = document.createElement("input");
            input.setAttribute("type", "hidden");
            input.setAttribute("name", "requisicao");
            input.setAttribute("value", "ativarEquipe");
            document.getElementById("formulario").appendChild(input);

            var input = document.createElement("input");
            input.setAttribute("type", "hidden");
            input.setAttribute("name", "uncid");
            input.setAttribute("value", "<?= $_SESSION['sispacto2'][$sis]['uncid'] ?>");
            document.getElementById("formulario").appendChild(input);

            document.getElementById('formulario').submit();
        }
    }

    function reiniciarSenha(cpf) {
        var conf = confirm('Deseja realmente ativar e reiniciar a senha deste usuário para "simecdti"?');

        if (conf) {
            window.location = window.location + '&requisicao=reiniciarSenha&usucpf=' + cpf;

        }
    }

    function trocarUsuarioPerfil(pflcod, iusd) {

        jQuery('#iusdantigo').val(iusd);
        jQuery('#pflcod_').val(pflcod);

        jQuery("#modalFormulario").dialog({
            draggable: true,
            resizable: true,
            width: 800,
            height: 600,
            modal: true,
            close: function() {
            }
        });


    }

    function inserirUsuarioPerfil() {

        jQuery("#modalFormulario2").dialog({
            draggable: true,
            resizable: true,
            width: 800,
            height: 600,
            modal: true,
            close: function() {
            }
        });


    }

    function carregaUsuario_(esp) {
        var usucpf = document.getElementById('iuscpf' + esp).value;

        usucpf = usucpf.replace('-', '');
        usucpf = usucpf.replace('.', '');
        usucpf = usucpf.replace('.', '');

        var comp = new dCPF();
        comp.buscarDados(usucpf);
        var arrDados = new Object();

        if (!comp.dados.no_pessoa_rf) {
            alert('CPF Inválido');
            return false;
        }

        document.getElementById('iusnome' + esp).value = comp.dados.no_pessoa_rf;

        jQuery.ajax({
            type: "POST",
            url: window.location.href,
            data: 'requisicao=pegarDadosUsuarioPorCPF&cpf=' + usucpf,
            async: false,
            success: function(dados) {
                var da = dados.split("||");
                document.getElementById('iusemailprincipal' + esp).value = da[0];
            }
        });

        divCarregado();
    }

    function efetuarTrocaUsuarioPerfil() {

        jQuery('#iuscpf_').val(mascaraglobal('###.###.###-##', jQuery('#iuscpf_').val()));

        if (jQuery('#iuscpf_').val() == '') {
            alert('CPF em branco');
            return false;
        }

        if (!validar_cpf(jQuery('#iuscpf_').val())) {
            alert('CPF inválido');
            return false;
        }

        if (jQuery('#iusnome_').val() == '') {
            alert('Nome em branco');
            return false;
        }

        if (jQuery('#iusemailprincipal_').val() == '') {
            alert('Email em branco');
            return false;
        }

        if (!validaEmail(jQuery('#iusemailprincipal_').val())) {
            alert('Email inválido');
            return false;
        }

        document.getElementById('formulario_troca').submit();

    }

    function carregarDetalhesPerfil(pflcod) {
        ajaxatualizar('requisicao=carregarDetalhesPerfil&uncid=<?= $_SESSION['sispacto2'][$sis]['uncid'] ?>&pflcod_=' + pflcod, 'detalhesperfil');
    }


    function efetuarInsercaoUsuarioPerfil() {

        jQuery('#iuscpf__').val(mascaraglobal('###.###.###-##', jQuery('#iuscpf__').val()));

        if (jQuery('#iuscpf__').val() == '') {
            alert('CPF em branco');
            return false;
        }

        if (!validar_cpf(jQuery('#iuscpf__').val())) {
            alert('CPF inválido');
            return false;
        }

        if (jQuery('#iusnome__').val() == '') {
            alert('Nome em branco');
            return false;
        }

        if (jQuery('#iusemailprincipal__').val() == '') {
            alert('Email em branco');
            return false;
        }

        if (!validaEmail(jQuery('#iusemailprincipal__').val())) {
            alert('Email inválido');
            return false;
        }

        if (document.getElementById('picid__')) {
            if (jQuery('#picid__').val() == '') {
                alert('Esfera em branco');
                return false;
            }
        }

        if (document.getElementById('turid__')) {
            if (jQuery('#turid__').val() == '') {
                alert('Turma em branco');
                return false;
            }
        }


        var input = document.createElement("input");
        input.setAttribute("type", "hidden");
        input.setAttribute("name", "uncid");
        input.setAttribute("value", "<?= $_SESSION['sispacto2'][$sis]['uncid'] ?>");
        document.getElementById("formulario_insercao").appendChild(input);

        document.getElementById('formulario_insercao').submit();

    }


    function excluirUsuarioPerfil(pflcod, iusd) {
        var conf = confirm('Deseja realmente excluir o este cargo? \n\n- Essa ação será definitiva e não poderá ser incluído um novo membro.\n- Caso este ja tenha recebido algum pagamento, o sistema não permitirá a exclusão\n- Ícone ao lado é uma ferramenta de substituição, tenha certeza de que ela não é a ferramenta que esta necessitando.');
        if (conf) {
            window.location = '<?= $_SERVER['REQUEST_URI'] ?>&requisicao=excluirUsuarioPerfil&pflcod=' + pflcod + '&iusd=' + iusd;
        }
    }

    function marcarTodos(obj) {
        jQuery("[name^='chk[']").attr('checked', obj.checked);
    }

    function atualizarEmail(iusd, email) {
        var fname = prompt("Digite novo e-mail:", email);
        if (fname) {
            if (fname == '') {
                alert('Email Principal em branco');
                return false;
            }
            if (!validaEmail(fname)) {
                alert('Email Principal inválido');
                return false;
            }
            ajaxatualizar('requisicao=atualizarEmail&iusd=' + iusd + '&iusemailprincipal=' + fname, '');

            document.getElementById('formulario').submit();
        }
    }

    function atualizarMunicipioAtuacao(iusd, muncod) {
        if (muncod == '') {
            alert('Selecione um Município de atuação');
            return false;
        }

        ajaxatualizar('requisicao=atualizarMunicipioAtuacao&iusd=' + iusd + '&muncod=' + muncod, '');

        document.getElementById('formulario').submit();

    }


    function exibirMunicipiosAtuacao(iuscpf) {
        ajaxatualizar('requisicao=exibirMunicipiosAtuacao&iuscpf=' + iuscpf, 'modalHistoricoUsuario');

        jQuery("#modalHistoricoUsuario").dialog({
            draggable: true,
            resizable: true,
            width: 800,
            height: 600,
            modal: true,
            close: function() {
            }
        });

    }



</script>

<div id="modalHistoricoUsuario" style="display:none;"></div>

<div id="modalFormulario2" style="display:none;">
    <form method="post" name="formulario_insercao" id="formulario_insercao">
        <input type="hidden" name="requisicao" value="efetuarInsercaoUsuarioPerfil">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubTituloDireita" width="25%">Orientações</td>
                <td>
                    <p>Esta é uma ferramenta de inserção de usuários direta, sem aprovações ou fluxos. Todas alterações são registradas no projeto, logando as informações de quem será substituido, do novo membro e do responsável pela mudança.</p>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">CPF</td>
                <td><?= campo_texto('iuscpf__', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf__"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'__\');}'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Nome</td>
                <td><?= campo_texto('iusnome__', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome__"', ''); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Email</td>
                <td><?= campo_texto('iusemailprincipal__', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal__"'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Perfil</td>
                <td><?
                    $sql = "SELECT p.pflcod as codigo, p.pfldsc as descricao FROM seguranca.perfil p INNER JOIN sisfor.pagamentoperfil pgp ON pgp.pflcod = p.pflcod ORDER BY p.pfldsc";
                    $db->monta_combo('pflcod__', $sql, 'S', 'Selecione', 'carregarDetalhesPerfil', '', '', '', 'N', 'pflcod__', '', $_REQUEST['pflcod__']);
                    ?></td>
            </tr>
            <tr>
                <td colspan="2" id="detalhesperfil"></td>
            </tr>

            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="salvar" value="Salvar" onclick="efetuarInsercaoUsuarioPerfil();">
                </td>
            </tr>
        </table>
    </form>
</div>


<div id="modalFormulario" style="display:none;">
    <form method="post" name="formulario_troca" id="formulario_troca">
        <input type="hidden" name="requisicao" value="efetuarTrocaUsuarioPerfil">
        <input type="hidden" name="iusdantigo" id="iusdantigo" value="">
        <input type="hidden" name="pflcod_" id="pflcod_" value="">
        <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
            <tr>
                <td class="SubTituloDireita" width="25%">Orientações</td>
                <td>
                    <p>Esta é uma ferramenta de troca de usuários direta, sem aprovações ou fluxos. Todas alterações são registradas no projeto, logando as informações de quem será substituido, do novo membro e do responsável pela mudança.</p>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">CPF</td>
                <td><?= campo_texto('iuscpf_', "S", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iuscpf_"', '', '', 'if(this.value!=\'\'){carregaUsuario_(\'_\');}'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Nome</td>
                <td><?= campo_texto('iusnome_', "S", "N", "Nome", 67, 150, "", "", '', '', 0, 'id="iusnome_"', ''); ?></td>
            </tr>
            <tr>
                <td class="SubTituloDireita" width="25%">Email</td>
                <td><?= campo_texto('iusemailprincipal_', "S", "S", "Principal", 67, 60, "", "", '', '', 0, 'id="iusemailprincipal_"'); ?></td>
            </tr>
            <tr>
                <td class="SubTituloCentro" colspan="2">
                    <input type="button" name="salvar" value="Salvar" onclick="efetuarTrocaUsuarioPerfil();">
                </td>
            </tr>
        </table>
    </form>
</div>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td class="SubTituloCentro" colspan="2">Gerenciar Equipe</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="20%">Orientações</td>
        <td>
<p>Esta tela permite gerenciar os acessos ou a substituição de todos os bolsistas. Para tanto, basta marcar o membro da equipe (clicando no checkbox) e clicar em alguns dos botões situados no final da lista, podendo ativar ou bloquear os acesso. Existem basicamente 4 situações de acesso:</p>
<p>Ativo - usuário esta autorizado a acessar o sistema;<br>
Pendente - usuário solicitou acesso pela página principal do <?php echo SIGLA_SISTEMA; ?>, e está aguardando confirmação;<br>
Bloqueado - usuário esta bloqueado, podendo ser pelo excesso de tentativas (5 tentativas de acesso) ou foi bloqueado por alguém com perfil superior;<br>
Não cadastrado - usuário não possui acesso ao SisFor. É possível visualizar o histórico do gerenciamento clicando no ícone colorido da situação.<br>
</p>
<p>Esta tela permite também alterar o e-mail do usuário clicando no ícone ao lado do e-mail. Para substituir as pessoas, basta clicar no ícone azul e inserir os dados referente ao novo bolsista. Observe que, neste caso, só serão pagas as bolsas previstas para quem foi substituído. Clicando no ícone amarelo, você pode ativar e reiniciar a senha do usuário para “simecdti”. Dúvidas  ou problemas com o sistema? Envie um e-mail para <?php echo $_SESSION['email_sistema']; ?>.</p>
        
        
        </td>
    </tr>
    <tr>
        <td colspan="2" valign="top">
            <form method=post name="formbuscar" id="formbuscar">
                <table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
                    <tr><td class="SubTituloDireita">CPF</td><td><?= campo_texto('iuscpf', "N", "S", "CPF", 15, 14, "###.###.###-##", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iuscpf']); ?></td></tr>
                    <tr><td class="SubTituloDireita">Nome</td><td><?= campo_texto('iusnome', "N", "S", "Nome", 67, 60, "", "", '', '', 0, 'id="iusnome"', '', $_REQUEST['iusnome']); ?></td></tr>
                    <tr>
                        <td class="SubTituloDireita">Perfil</td>
                        <td>
                            <?
//$sql = "SELECT pfldsc as codigo, pfldsc as descricao FROM seguranca.perfil WHERE pflcod IN('".PFL_FORMADORIESP."','".PFL_FORMADORIES."','".PFL_SUPERVISORIES."','".PFL_COORDENADORADJUNTOIES."','".PFL_COORDENADORLOCAL."','".PFL_ORIENTADORESTUDO."','".PFL_PROFESSORALFABETIZADOR."','".PFL_COORDENADORIES."') ORDER BY pfldsc";
//$db->monta_combo('pfldsc', $sql, 'S', 'Selecione', 'selecionarPerfilGerenciar', '', '', '200', 'S', 'fpbid','', $_REQUEST['pfldsc']); 
                            ?> 
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Situação</td>
                        <td>
                            <?
                            $arrSituacao = array(0 => array("codigo" => "A", "descricao" => "Ativo"),
                                1 => array("codigo" => "B", "descricao" => "Bloqueado"),
                                2 => array("codigo" => "P", "descricao" => "Pendente"),
                                3 => array("codigo" => "N", "descricao" => "Não Cadastrado")
                            );
                            $db->monta_combo('status', $arrSituacao, 'S', 'Selecione', '', '', '', '200', 'S', 'status', '', $_REQUEST['status']);
                            ?>
                        </td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">UF</td>
                        <td><?
                            $sql = "SELECT estuf as codigo, estuf as descricao FROM territorios.estado ORDER BY estuf";
                            $db->monta_combo('estuf_endereco', $sql, 'S', 'Selecione', 'carregarMunicipiosPorUF3', '', '', '', 'S', 'estuf_endereco', '');
                            ?></td>
                    </tr>
                    <tr>
                        <td class="SubTituloDireita">Município</td>
                        <td id="td_municipio3">
                            <?
                            if ($muncod_endereco) :
                                $sql = "SELECT mundescricao as descricao FROM territorios.municipio WHERE muncod='" . $muncod_endereco . "'";
                                $mundescricao = $db->pegaUm($sql);
                                echo $mundescricao;
                                echo '<input type="hidden" id="muncod_endereco" name="muncod_endereco" value="' . $muncod_endereco . '">';
                            else :
                                echo "Selecione UF";
                            endif;
                            ?>
                        </td>
                    </tr>

                    <tr><td class=SubTituloCentro colspan=2><input type="button" name="buscar" value="Buscar" onclick="document.getElementById('formbuscar').submit();"><input type="button" name="vertodos" value="Ver todos" onclick="window.location = window.location;"></td></tr>
                    <tr>
                        <td class=SubTituloEsquerda>
                            <input type="checkbox" id="marcartodos" onclick="marcarTodos(this);"> Marcar todos
                        </td>
                        <td class=SubTituloEsquerda>
                            <? $perfis = pegaperfilGeral(); ?>
                            <? if ((in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_SUPERUSUARIO, $perfis)) && $sis == 'mec') : ?>
                                <img src="../imagens/gif_inclui.gif" style="cursor:pointer;" align="absmiddle" onclick="inserirUsuarioPerfil();"> Inserir bolsista
                            <? endif; ?> 
                        </td>
                    </tr>
                </table>
            </form>
            <?
            $perfis = pegaPerfilGeral();

            if ($_REQUEST['iuscpf']) {
                $where[] = "foo.iuscpf='" . str_replace(array(".", "-"), array("", ""), $_REQUEST['iuscpf']) . "'";
            }

            if ($_REQUEST['pfldsc']) {
                $where[] = "foo.pfldsc='" . $_REQUEST['pfldsc'] . "'";
            }

            if ($_REQUEST['estuf_endereco']) {
                $rede .= $_REQUEST['estuf_endereco'] . " / ";
            }

            if ($_REQUEST['muncod_endereco']) {
                $rede .= $db->pegaUm("SELECT mundescricao FROM territorios.municipio WHERE muncod='" . $_REQUEST['muncod_endereco'] . "'");
            }

            if ($_REQUEST['status']) {
                if ($_REQUEST['status'] == "N") {
                    $where[] = "(foo.status IS NULL OR foo.perfil IS NULL)";
                } else {
                    $where[] = "((foo.status='" . $_REQUEST['status'] . "' OR usu.suscod='" . $_REQUEST['status'] . "'))";
                }
            }

            $sql_equipe = "SELECT
                                        iu.iuscpf, 
					iu.iusnome, 
					iu.iusemailprincipal,
            		iu.iustermocompromisso, 
					p.pflcod,
					p.pfldsc, 
                                        iu.iusd,
					(SELECT suscod FROM seguranca.usuario_sistema WHERE usucpf=iu.iuscpf AND sisid=" . SIS_SISFOR . ") as status,
					(SELECT usucpf FROM seguranca.perfilusuario WHERE usucpf=iu.iuscpf AND pflcod=tp.pflcod) as perfil
                    from sisfor.tipoperfil tp
                    inner join sisfor.identificacaousuario iu on iu.iusd = tp.iusd
                    inner join sisfor.equipeies e on e.pflcod = tp.pflcod
                    inner join seguranca.perfil p on p.pflcod = e.pflcod
                    where e.sifid = '{$_SESSION['sisfor']['sifid']}' and tp.sifid = '{$_SESSION['sisfor']['sifid']}'";


            $sql = "SELECT '<input type=\"checkbox\" name=\"chk['||foo.pflcod||'][]\" value=\"'||foo.iuscpf||'\"> '|| CASE WHEN foo.status IS NOT NULL AND foo.perfil IS NOT NULL THEN '<img src=\"../imagens/icon_campus_1.png\" border=\"0\" style=\"cursor:pointer;\" onclick=\"reiniciarSenha(\''||foo.iuscpf||'\');\" onmouseover=\"return escape(\'Ativar usuário e reiniciar senha\');\">' ELSE '' END  as acao, 
		    CASE WHEN foo.status='A' AND foo.perfil IS NOT NULL AND usu.suscod='A' THEN '<img src=\"../imagens/p_verde.gif\" border=\"0\" align=\"absmiddle\"\">' ELSE '<img src=\"../imagens/p_vermelho.gif\" border=\"0\" align=\"absmiddle\" \">' END ||'
		   '|| CASE WHEN foo.status IS NULL OR foo.perfil IS NULL THEN 'Não Cadastrado'
                                   		WHEN (foo.status='A' AND usu.suscod='A') THEN 'Ativo'
				   		WHEN (foo.status='B' OR usu.suscod='B')	 THEN 'Bloqueado' 
				   		WHEN (foo.status='P' OR usu.suscod='P')  THEN 'Pendente' END as situacao,
				   replace(to_char(foo.iuscpf::numeric, '000:000:000-00'), ':', '.') as iuscpf, 
				   foo.iusnome, 
				   foo.iusemailprincipal || ' <img src=../imagens/arrow_v.png style=cursor:pointer; align=absmiddle onclick=\"atualizarEmail('||foo.iusd||',\''||foo.iusemailprincipal||'\');\">' as iusemailprincipal, 
				   foo.pfldsc,
				   CASE WHEN foo.iustermocompromisso=true THEN '<span style=color:blue;>Sim</span>' ELSE '<span style=color:red;>Não</span>' END as termo 
			FROM (
			(
			
			{$sql_equipe}
			
			)
			 
			) foo 
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = foo.iuscpf" . (($where) ? " WHERE " . implode(" AND ", $where) : "") . " ORDER BY foo.iusnome";

            $cabecalho = array("&nbsp;", "Situação", "CPF", "Nome", "E-mail", "Perfil", "Termo");
            $db->monta_lista($sql, $cabecalho, 100, 10, 'N', 'center', 'N', 'formulario', '', '', null, array('ordena' => false));
            ?>
        </td>
    </tr>
    <tr>
        <td colspan="2" align="center">
            <input type="button" id="ativarmarcados" value="Ativar Marcados" onclick="enviarMarcado('A');">
            <input type="button" id="bloquearmarcados" value="Bloquear Marcados" onclick="enviarMarcado('B');">
        </td>
    </tr>
</table>