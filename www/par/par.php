<?php
ini_set("memory_limit", "2024M");
set_time_limit(0);

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/classes/Controle.class.inc";
include_once APPRAIZ . "includes/classes/Visao.class.inc";
include_once APPRAIZ . "includes/library/simec/Listagem.php";


// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoesPar.php';
include_once '../obras2/_funcoes_obras_par.php';
include_once '_funcoes_painel.php';
include_once '_componentes.php';
include_once 'autoload.php';

simec_magic_quotes();
/* if( gettype( $db->link ) != 'resource' ){
	$db = new cls_banco(); // corrigindo a perda do link do $db
} */
$arrPerfil = pegaPerfilGeral();
$arrPerfil = $arrPerfil ? $arrPerfil : array();
if ($_REQUEST['importarUsuarios']) {
    //equipe municipal 1344                 460
    //equipe estadual 1345                  461
    //prefeito 1349                         556
    //Secretario estadual 1350              672

    $perfis[0]["novo"] = 1344;
    $perfis[0]["antigo"] = 460;

    $perfis[1]["novo"] = 1345;
    $perfis[1]["antigo"] = 461;

    $perfis[2]["novo"] = 1349;
    $perfis[2]["antigo"] = 556;

    $perfis[3]["novo"] = 1350;
    $perfis[3]["antigo"] = 672;

    foreach ($perfis as $key) {

        $sql = " SELECT DISTINCT
		   		usuario.usucpf,
		   		usuario.usunome as nomeusuario,
		   		'(' || usuario.usufoneddd || ') '
		   		|| usuario.usufonenum as fone ,
		   		usuario.regcod,
		   		municipio.mundescricao,
		   		entidade.entnome as orgao,
				CASE WHEN entidade.entid = 390402 THEN trim(usuario.orgao) ELSE trim(unidadex.unidsc) END as unidsc,
		   		COALESCE(cargo.cardsc,'')||' / '||COALESCE(usuario.usufuncao,'') as cargo,
				to_char(usuario.usudataatualizacao,'dd/mm/YYYY HH24:MI') as data
			FROM
				seguranca.perfil perfil
				inner join seguranca.perfilusuario perfilusuario    on perfil.pflcod = perfilusuario.pflcod and perfil.pflcod = " . $key["antigo"] . "
				right join seguranca.usuario usuario		    	on usuario.usucpf = perfilusuario.usucpf
				left join
				(
				  select
					unicod,
					unidsc
				  from
					public.unidade
				  where
					unitpocod = 'U'
				) as unidadex on usuario.unicod = unidadex.unicod
			left join  territorios.municipio municipio 	    	on municipio.muncod = usuario.muncod
			inner join seguranca.usuario_sistema usuariosistema on usuario.usucpf = usuariosistema.usucpf
			left join  entidade.entidade entidade		    	on usuario.entid = entidade.entid
			left join  public.cargo cargo		    			on cargo.carid = usuario.carid
			WHERE
				usunome is not null  and usuariosistema.suscod = 'A' and usuariosistema.sisid = '23' and (perfil.pflcod = " . $key["antigo"] . ")
			GROUP BY
				usuario.usucpf, usuario.usunome, usuario.usufoneddd,
				usuario.usufonenum, usuario.regcod, entidade.entid, entidade.entnome,
				unidadex.unidsc, usuario.orgao, usuario.usudataatualizacao , municipio.mundescricao, cargo.cardsc, usuario.usufuncao
			ORDER BY
				nomeusuario";


        $dados2 = $db->carregar($sql);

        //ver($dados2,d);

        foreach ($dados2 as $key2) {
            $sql_insert[] = "insert into seguranca.perfilusuario (usucpf,pflcod) values ('{$key2['usucpf']}',{$key["novo"]});";
            $sql3 = "select * from par.usuarioresponsabilidade where usucpf = '{$key2['usucpf']}' and pflcod = {$key["antigo"]} and rpustatus = 'A'";


            $dados3 = $db->carregar($sql3);
            if (!empty($dados3)) {
                foreach ($dados3 as $key3) {
                    $sql_insert[] = "insert into par3.usuarioresponsabilidade (usucpf,pflcod,estuf,muncod,prgid,entid,rpustatus) values ('{$key2['usucpf']}',{$key["novo"]},'{$key3["estuf"]}',{$key3["muncod"]},{$key3["prgid"]},{$key3["entid"]},'A');";
                }
            }
        }

    }

    ver($sql_insert, d);


}


$boVisualizaTudo = false;
if (in_array(PAR_PERFIL_SUPER_USUARIO, $arrPerfil) || in_array(PAR_PERFIL_ADMINISTRADOR, $arrPerfil)) {
    $boVisualizaTudo = true;
}

// Painel do Administrador e Super Usuário
if ($boVisualizaTudo || $db->testa_superuser()) {
    $painelCabecalho = array(
        array('titulo' => 'WorkFlow', 'funcao' => 'montarPainelWorkflow', 'icon' => 'tasks'),
    );
}

// Painel do perfil CONSULTA GERAL
if ($boVisualizaTudo || in_array(PAR_PERFIL_CONSULTA, $arrPerfil)) {
    $painelCabecalho[] = array('titulo' => 'Base Nacional Comum', 'funcao' => 'montarPainelBaseNacional', 'icon' => 'tasks');
}


/**
 * @TODO Tratamento para colocar o layout antigo nas telas de sistemas que não tem o jquery compativel ainda com o layout novo
 */
$arrModulo = explode('/', $_GET['modulo']);
//$modulo = reset($arrModulo);
$modulo = $arrModulo[1];
if (!empty($modulo) && $modulo == 'painel') {
    $_SESSION['sislayoutbootstrap'] = true;
} else {
    $_SESSION['sislayoutbootstrap'] = false;
}

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

prepararDetalheProcesso();
prepararDetalhePendenciasObras();
prepararDetalheFuncionalProgramatica();
verificaPendenciasDemandas();
?>